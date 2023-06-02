/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <Checker.h>
#include <utils/Configuration.h>
#include <utils/ContextPool.h>
#include <utils/LocaleUtils.h>

#include <algorithm>

#include "HastMatchingEngine.h"
#include "SupportedLanguages.h"

namespace {

//  LCOV_EXCL_START
//  TODO: add test to cover it under HCAT-143
// COODDY_SUPPRESS
void ReportAboutIncorrectHastExpression(const std::string& expression, const std::string& errorText)
{
    HCXX::Log(HCXX::LogLevel::ERROR) << "HAST_expression [%1$s] isn't correct"
                                     << ". Error is " << errorText << std::endl;
}
//  LCOV_EXCL_STOP

struct ConfigurationItem {
    std::string expression;
    std::string inspectionCode;
    std::string inspectionText;
    HCXX::Problem::Severity severity = HCXX::Problem::Severity::ERROR;
    HCXX::SupportedLanguages languages;

    DECLARE_FIELDS("expression", expression, "kind", inspectionCode, "inspection_text", inspectionText, "severity",
                   MakeTypeSerializer(severity, HCXX::Problem::SeverityFromStrWithCheck), "lang",
                   MakeTypeSerializer(languages, HCXX::SupportedLanguages::FromArr, std::vector<std::string>()))
};

using CheckerConfiguration = std::vector<ConfigurationItem>;

struct SubChecker {
    HCXX::HastExpressionPtr expression;
    std::string inspectionCode;
    std::string inspectionMessage;
    HCXX::Problem::Severity severity = HCXX::Problem::Severity::ERROR;

    SubChecker() = default;
    SubChecker(HCXX::HastExpressionPtr&& expression, std::string_view inspectionCode,
               std::string_view inspectionMessage, HCXX::Problem::Severity severity)
        : expression(std::move(expression)),
          inspectionCode(inspectionCode),
          inspectionMessage(inspectionMessage),
          severity(severity)
    {}
    ~SubChecker() = default;

    //  LCOV_EXCL_START
    SubChecker(SubChecker&&) noexcept = default;
    SubChecker& operator=(SubChecker&&) noexcept = default;
    //  LCOV_EXCL_STOP

    SubChecker(const SubChecker&) = delete;
    SubChecker& operator=(const SubChecker&) = delete;
};

class ThreadSpecificContext {
public:
    ThreadSpecificContext() {}

    ThreadSpecificContext(const ThreadSpecificContext&) = delete;
    ThreadSpecificContext(ThreadSpecificContext&&) = delete;

    ThreadSpecificContext& operator=(const ThreadSpecificContext&) = delete;
    ThreadSpecificContext& operator=(ThreadSpecificContext&&) = delete;

    ~ThreadSpecificContext() {}

    void MatchAll(const HCXX::TranslationUnit& tu)
    {
        myEngine.MatchAll(tu);
    }

    // TODO HCAT-305
    // COODDY_SUPPRESS ParameterNumberChecker
    bool AddExpression(const std::string_view hastExpression, std::string_view inspectionCode,
                       std::string_view inspectionMessage, HCXX::Problem::Severity severity,
                       HCXX::SupportedLanguages languages, std::string* errorMsg)
    {
        auto ptr = myEngine.BuildExpression(hastExpression, languages, errorMsg);
        if (!ptr) {
            return false;  // LCOV_EXCL_LINE: incorrect expression
        }

        mySubCheckers.emplace_back(std::move(ptr), inspectionCode, inspectionMessage, severity);
        return true;
    }

    template <typename L>
    void ForEachSubChecker(const L& l) const
    {
        for (const SubChecker& item : mySubCheckers) {
            l(item);
        }
    }

private:
    HCXX::HastMatchingEngine myEngine;
    std::vector<SubChecker> mySubCheckers;
};

class ContextFactory {
public:
    using FactoryObject = ThreadSpecificContext;
    using FactoryObjectPtr = std::unique_ptr<FactoryObject>;
    using Cfg = CheckerConfiguration;

    FactoryObjectPtr CreateContext() const
    {
        auto context = std::make_unique<ThreadSpecificContext>();

        for (const auto& item : myCfg) {
            std::string errorMsg;
            if (!context->AddExpression(item.expression.c_str(), item.inspectionCode.c_str(),
                                        item.inspectionText.c_str(), item.severity, item.languages, &errorMsg)) {
                //  LCOV_EXCL_START
                //  The code isn't reachable because we have already checked the configuration
                static bool isFirst = true;
                if (isFirst) {
                    isFirst = false;
                    ReportAboutIncorrectHastExpression(item.expression, errorMsg);
                }
                //  LCOV_EXCL_STOP
            }
        }

        return context;
    }

    void Configure(Cfg&& cfg)
    {
        myCfg = std::move(cfg);
        //  We want to detect incorrect expressions as soon as possible
        //  So we are checking the configuration
        HCXX::HastMatchingEngine myEngine;
        auto isCfgSubItemInvalid = [&myEngine](const ConfigurationItem& item) {
            std::string errorMsg;
            auto ptr = myEngine.BuildExpression(item.expression.c_str(), HCXX::SupportedLanguages(), &errorMsg);
            if (ptr) {
                //  The expression is valid => it mustn't be removed => return false
                return false;
            }

            //  LCOV_EXCL_START
            //  TODO: add test to cover it under HCAT-143
            ReportAboutIncorrectHastExpression(item.expression, errorMsg);
            //  The expression isn't valid => it must be removed => return true
            return true;
            //  LCOV_EXCL_STOP
        };

        myCfg.erase(std::remove_if(myCfg.begin(), myCfg.end(), isCfgSubItemInvalid), myCfg.end());
    }

private:
    Cfg myCfg;
};

}  // namespace

namespace HCXX {

class HastBasedChecker : public Checker {
public:
    HastBasedChecker() : myPool(std::make_unique<ContextFactory>()) {}

    virtual void CheckNode(const Node& node, ProblemsHolder& holder)
    {
        const auto* tu = Node::Cast<TranslationUnit>(&node);
        if (tu == nullptr) {
            return;
        }
        auto context(myPool.AllocateContext());

        context->MatchAll(*tu);

        context->ForEachSubChecker([&](const SubChecker& sc) {
            const std::set<const Node*>& matched = sc.expression->GetMatchedResult();
            for (const Node* matchedNode : matched) {
                Problem problem;
                problem.kind = sc.inspectionCode;
                problem.description = sc.inspectionMessage;
                problem.severity = sc.severity;
                holder.RegisterProblem(*this, *tu, matchedNode->GetRange(), std::move(problem));
            }
        });
    }

    virtual bool Initialize(const Checker::Config& config)
    {
        CheckerConfiguration cfg;

        if (!LoadConfiguration(cfg, config, GetName())) {
            //  LCOV_EXCL_START
            return false;
            //  LCOV_EXCL_STOP
        }

        constexpr int preallocatedPoolSize = 5;
        myPool.Configure(preallocatedPoolSize, std::move(cfg));
        return true;
    }

private:
    ContextPool<ContextFactory> myPool;
};

}  // namespace HCXX

std::unique_ptr<HCXX::Checker> CreateHastBasedChecker()
{
    return std::make_unique<HCXX::HastBasedChecker>();
}
