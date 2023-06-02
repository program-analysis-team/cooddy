/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef HCXXANALYZER_ANALYZER_H
#define HCXXANALYZER_ANALYZER_H

#include <Checker.h>
#include <Parser.h>
#include <workspace/Workspace.h>

#include <memory>
#include <string>
#include <vector>

namespace HCXX {

class CompilerOptionsList;
class DfaFunctionContext;
struct SuspiciousPath;

class Analyzer {
    // LCOV_EXCL_START
public:
    static std::unique_ptr<Analyzer> Create(Parser& parser, Workspace& workspace);

    struct Consumer : public Parser::Consumer {
        virtual void OnAnalysisBegin(TranslationUnit& unit){};

        virtual void OnParseEnd(TranslationUnit& unit){};

        virtual bool ShouldParseUnit(TranslationUnit& unit, Parser::ParseStages parseStage)
        {
            return true;
        };

        virtual bool ShouldAnalyzeUnit(TranslationUnit& unit)
        {
            return true;
        };

        virtual bool ShouldAnalyzeFunction(DfaFunctionContext& functionContext)
        {
            return true;
        };

        virtual bool ShouldCheckPath(DfaFunctionContext& functionContext, const SuspiciousPath& path)
        {
            return true;
        };

        virtual void OnAnalysisEnd(TranslationUnit& unit){};
    };

    virtual void Analyze(const CompilerOptionsList& unitsOptions, ProblemsHolder& holder, Consumer& consumer) = 0;

    virtual void AnalyzeAsync(const CompilerOptionsList& unitsOptions, std::shared_ptr<ProblemsHolder> holder,
                              std::shared_ptr<Consumer> consumer = nullptr) = 0;

    virtual ~Analyzer() = default;
    // LCOV_EXCL_STOP
};

}  // namespace HCXX

#endif  // HCXXANALYZER_ANALYZER_H
