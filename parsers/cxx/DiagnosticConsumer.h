/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.txt.
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
        : myUnit(unit), myErrorLevel(HCXX::LogLevel::TRACE), myParserStatistics(stat)
    {}
    ~DiagnosticConsumer() = default;

private:
    static constexpr uint32_t MAX_LOG_ERROR_AFTER_FATAL = 20;

    std::vector<std::unique_ptr<CommentCallback>> myCmntCallbacks;
    HCXX::TranslationUnit* myUnit;
    HCXX::Parser::ParserStatistics* myParserStatistics;
    HCXX::LogLevel myErrorLevel;
    bool myFatalOccurred = false;

    /// A block of code with a specified line, with several lines before and after included for context.
    std::string GetSourceTextLine(const std::string& filePath, int line, int linesOfContext)
    {
        auto file = HCXX::FileEntriesCache::GetInstance().GetFileEntry(filePath);

        if (file == nullptr || file->lineOffsets.empty() || line >= file->lineOffsets.size()) {
            return "";  // LCOV_EXCL_LINE
        }

        auto& lineOffsets = file->lineOffsets;

        int startPos = (line - linesOfContext) >= 0 ? lineOffsets[line - linesOfContext] : lineOffsets[0];
        int endPos = (line + linesOfContext) >= lineOffsets.size() ? lineOffsets[lineOffsets.size() - 1]
                                                                   : lineOffsets[line + linesOfContext];

        return file->fileSource.substr(startPos, endPos - startPos);
    }

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

    void HandleDiagnostic(clang::DiagnosticsEngine::Level diagLevel, const clang::Diagnostic& info) override
    {
        HCXX::LogLevel logLevel = HCXX::LogLevel::OFF;
        switch (diagLevel) {
            case clang::DiagnosticsEngine::Fatal:
                logLevel = HCXX::LogLevel::FATAL;
                break;
            case clang::DiagnosticsEngine::Error:
                logLevel = HCXX::LogLevel::ERROR;
                break;
            default:
                return;
        }
        clang::SmallString<100> messageStr;
        info.FormatDiagnostic(messageStr);

        clang::PresumedLoc presumedLoc;
        if (info.getLocation().isValid() && info.hasSourceManager()) {
            presumedLoc = info.getSourceManager().getPresumedLoc(info.getLocation());
        }
        if (presumedLoc.isValid()) {
            auto sourceOfError = GetSourceTextLine(presumedLoc.getFilename(), presumedLoc.getLine(), 5);

            HCXX::Log(HCXX::LogLevel::PARSE_ERROR) << sourceOfError << '\n';

            if (!myFatalOccurred || myFatalOccurred && myUnit->GetParseErrors().size() < MAX_LOG_ERROR_AFTER_FATAL) {
                HCXX::Log(logLevel) << "File: " << presumedLoc.getFilename() << ", "
                                    << "Line: " << presumedLoc.getLine() << ", "
                                    << "Pos: " << presumedLoc.getColumn() << ", " << messageStr.c_str() << std::endl;
            }
            if (myUnit) {
                myUnit->AddParseError(HCXX::ConvertLocation(info.getSourceManager(), info.getLocation()));
            }
        } else {
            // LCOV_EXCL_START
            if (auto pos = messageStr.find("unknown argument"); pos != std::string::npos) {
                if (auto open = messageStr.find("'", pos + 16); open != std::string::npos) {
                    auto close = messageStr.find("'", open + 1);
                    std::unique_lock<std::mutex> lock(myParserStatistics->mutex);
                    myParserStatistics->unknownOptionsSet.emplace(messageStr.substr(open + 1, close - open - 1));
                }
                logLevel = HCXX::LogLevel::WARNING;
            }
            HCXX::Log(logLevel) << "CLang message: " << messageStr.c_str() << std::endl;
            // LCOV_EXCL_STOP
        }

        if (myErrorLevel > logLevel) {
            myErrorLevel = logLevel;
            if (myErrorLevel == HCXX::LogLevel::FATAL && myParserStatistics != nullptr) {
                ++myParserStatistics->fatalErrorCount;
            }
        }

        if (logLevel == HCXX::LogLevel::FATAL) {
            // COODDY_SUPPRESS HastBasedChecker
            const_cast<clang::DiagnosticsEngine*>(info.getDiags())->Reset();
            myFatalOccurred = true;
        }
    }
};

#endif  // COODDY_ANALYZER_SOURCE_CLANG_FACADE_DIAGNOSTICCONSUMER_H_
