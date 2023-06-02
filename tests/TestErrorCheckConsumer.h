/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_TESTERRORCHECKCONSUMER_H
#define COODDY_TESTERRORCHECKCONSUMER_H

#include "Analyzer.h"
#include "utils/StrUtils.h"

#ifdef _MSC_VER
#define THREAD_LOCAL __declspec(thread)
#else
#define THREAD_LOCAL __thread
#endif

class TestErrorCheckConsumer : public HCXX::Analyzer::Consumer {
    std::string myToolLog;
    bool myHasParserError = false;
    static THREAD_LOCAL TestErrorCheckConsumer* myCurrentTest;

public:
    static TestErrorCheckConsumer* GetCurrentTest()
    {
        return myCurrentTest;
    }

    static void SetAsCurrent(TestErrorCheckConsumer* consumer)
    {
        myCurrentTest = consumer;
    }

    TestErrorCheckConsumer()
    {
        SetAsCurrent(this);
    }

    ~TestErrorCheckConsumer()
    {
        ClearToolError();
        SetAsCurrent(nullptr);
    }

    void AddTraceSymbol(char symbol)
    {
        myToolLog += symbol;
    }

    void OnAnalysisBegin(HCXX::TranslationUnit& unit) override
    {
        SetAsCurrent(this);
    }

    /// Find text in recorded tool errors by partial match.
    bool ContainsToolError(std::string_view text) const
    {
        return myToolLog.find(text) != myToolLog.npos;
    }

    void ClearToolError()
    {
        if (!myToolLog.empty()) {
            std::cout << myToolLog;
            std::cout.flush();
            myToolLog.clear();
        }
    }

    bool HasToolError() const
    {
        return ContainsToolError("ERROR") || ContainsToolError("FATAL");
    }

    void OnParseError() override
    {
        myHasParserError = true;
    }

    bool HasParseError() const
    {
        return myHasParserError;
    }
};

#endif  // COODDY_TESTERRORCHECKCONSUMER_H
