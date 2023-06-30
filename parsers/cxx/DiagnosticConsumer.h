/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
//
// class DiagnosticConsumer implements logging of Clang compile errors and
// set PPCallbacks to clang::Preprocessor
//
#ifndef COODDY_ANALYZER_SOURCE_CLANG_FACADE_DIAGNOSTICCONSUMER_H_
#define COODDY_ANALYZER_SOURCE_CLANG_FACADE_DIAGNOSTICCONSUMER_H_

#include <utils/LocaleUtils.h>

#include "CommentCallback.h"

class DiagnosticConsumer : public clang::DiagnosticConsumer {
public:
    explicit DiagnosticConsumer(HCXX::TranslationUnit* unit, HCXX::Parser::ParserStatistics* stat = nullptr)
        : myUnit(unit), myParserStatistics(stat)
    {}
    ~DiagnosticConsumer() = default;

    uint32_t GetFatalErrorCount() const
    {
        return myFatalErrorCount;
    }

private:
    using ClangString = clang::SmallString<100>;
    static constexpr uint32_t MAX_LOG_ERROR_COUNT = 20;

    std::vector<std::unique_ptr<CommentCallback>> myCmntCallbacks;
    HCXX::TranslationUnit* myUnit;
    HCXX::Parser::ParserStatistics* myParserStatistics;
    uint32_t myFatalErrorCount = 0;

    void BeginSourceFile(const clang::LangOptions& langOpts, const clang::Preprocessor* preprocessor) override
    {
        if (myUnit != nullptr) {
            //  HCAT-455: Get rid of const_cast
            //  COODDY_SUPPRESS HastBasedChecker
            const_cast<clang::Preprocessor*>(preprocessor)
                ->addPPCallbacks(std::make_unique<PreProcessorCallbacks>(*myUnit, preprocessor));
            myCmntCallbacks.push_back(std::make_unique<CommentCallback>(*myUnit));
            //  HCAT-455: Get rid of const_cast
            //  COODDY_SUPPRESS HastBasedChecker
            const_cast<clang::Preprocessor*>(preprocessor)->addCommentHandler(myCmntCallbacks.back().get());
        }
    }

    bool IsSkippedMessage(ClangString& message)
    {
        static const std::array<const char*, 8> excludedMessages = {
            "variable-sized object may not be initialized",
            "'decltype(auto)' cannot be combined with other type specifiers",
            "initializing wide char array with incompatible wide string literal",
            "in capture list does not name a variable",
            "must be initialized by a constant expression",
            "reference to local binding",
            "no type named 'align_val_t' in namespace 'std'",
            "unknown register name"};

        for (auto& it : excludedMessages) {
            if (message.find(it) != std::string::npos) {
                return true;
            }
        }
        return false;
    }

    bool IsUnknownArgumentMessage(ClangString& message)
    {
        static const char unknownArg[] = "unknown argument";

        auto pos = message.find(unknownArg);
        if (pos == std::string::npos) {
            return false;
        }
        if (auto open = message.find("'", pos + sizeof(unknownArg)); open != std::string::npos) {
            auto close = message.find("'", open + 1);
            std::unique_lock<std::mutex> lock(myParserStatistics->mutex);
            myParserStatistics->unknownOptionsSet.emplace(message.substr(open + 1, close - open - 1));
        }
        HCXX::Log(HCXX::LogLevel::WARNING) << "CLang: " << message.c_str() << std::endl;
        return true;
    }

    std::string GetIssueReason(const std::string& errorMessage)
    {
        if (HCXX::StrUtils::StartsWith(errorMessage, "error reading")) {
            return "source file not found";
        }
        if (errorMessage.find("file not found") != std::string::npos) {
            return "header file not found";
        }
        auto compilerPaths = HCXX::EnvironmentUtils::GetExecutablePaths(myUnit->GetCompilerOptions().compiler, 1);
        if (compilerPaths.empty() || compilerPaths[0].empty() || !std::filesystem::exists(compilerPaths[0])) {
            return "compiler not found";
        }
        return "compilation issue";
    }

    void HandleDiagnostic(clang::DiagnosticsEngine::Level diagLevel, const clang::Diagnostic& info) override
    {
        HCXX::LogLevel logLevel = HCXX::LogLevel::OFF;
        if (diagLevel == clang::DiagnosticsEngine::Fatal) {
            logLevel = HCXX::LogLevel::FATAL;
        } else if (diagLevel == clang::DiagnosticsEngine::Error) {
            logLevel = HCXX::LogLevel::ERROR;
        } else {
            return;
        }
        ClangString message;
        info.FormatDiagnostic(message);

        clang::PresumedLoc presumedLoc;
        if (info.getLocation().isValid() && info.hasSourceManager()) {
            presumedLoc = info.getSourceManager().getPresumedLoc(info.getLocation());
        }
        if (IsSkippedMessage(message) || !presumedLoc.isValid() && IsUnknownArgumentMessage(message)) {
            return;
        }
        if (myUnit->GetParseErrors().size() < MAX_LOG_ERROR_COUNT) {
            HCXX::Log(logLevel) << "CLang: " << (presumedLoc.isValid() ? presumedLoc.getFilename() : "<unknown>") << ":"
                                << (presumedLoc.isValid() ? presumedLoc.getLine() : 0) << ":"
                                << (presumedLoc.isValid() ? presumedLoc.getColumn() : 0) << ": " << message.c_str()
                                << std::endl;
            std::unique_lock<std::mutex> lock(myParserStatistics->mutex);
            auto& issue = myParserStatistics->compilationIssues[myUnit];
            if (issue.reason.empty()) {
                issue.reason = GetIssueReason(message.c_str());
            }
            issue.errors.emplace_back(message.c_str());
        }
        if (myUnit) {
            myUnit->AddParseError(HCXX::ConvertLocation(info.getSourceManager(), info.getLocation()));
        }
        if (logLevel == HCXX::LogLevel::FATAL && ++myFatalErrorCount <= myParserStatistics->maxFatalErrorCount) {
            // COODDY_SUPPRESS HastBasedChecker
            const_cast<clang::DiagnosticsEngine*>(info.getDiags())->Reset();
        }
    }
};

#endif  // COODDY_ANALYZER_SOURCE_CLANG_FACADE_DIAGNOSTICCONSUMER_H_
