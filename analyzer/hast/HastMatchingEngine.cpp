/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include "HastMatchingEngine.h"

#include <TranslationUnit.h>

#include <list>

#include "HastExpressionImpl.h"

namespace HCXX {

class HastMatchingEngine::Impl {
public:
    HastExpressionPtr BuildExpression(const std::string_view expression, HastMatchingEngine& parent,
                                      SupportedLanguages languages, std::string* errorMsg)
    {
        std::unique_ptr<Hast::Details::HastExpressionImpl> ptr =
            Hast::Details::HastExpressionImpl::BuildExpression(expression, errorMsg);
        if (!ptr) {
            return HastExpressionPtr();
        }
        Hast::Details::HastExpressionImpl* raw = ptr.get();
        myExpressions.emplace_back(std::move(ptr));
        if (languages.IsPureCLanguageSupported()) {
            myPureCExpressions.emplace_back(raw);
        }
        if (languages.IsCppLanguageSupported()) {
            myCppExpressions.emplace_back(raw);
        }
        return HastExpressionPtr(raw, std::bind(&HastMatchingEngine::DeleteExpression, &parent, std::placeholders::_1));
    }

    void DeleteExpression(const HastExpression* ptr)
    {
        auto it = std::find_if(myExpressions.begin(), myExpressions.end(),
                               [ptr](const auto& item) { return item.get() == ptr; });
        if (it == myExpressions.end()) {
            return;
        }

        auto raw = it->get();
        myExpressions.erase(it);
        myPureCExpressions.remove(raw);
        myCppExpressions.remove(raw);
    }

    void MatchAll(const TranslationUnit& node)
    {
        ForEachExpression(myExpressions,
                          [](std::unique_ptr<Hast::Details::HastExpressionImpl>& expr) { expr->ClearMatches(); });

        auto& cont = (node.GetLanguage() == UnitLanguage::CPP ? myCppExpressions : myPureCExpressions);
        ForEachExpression(cont,
                          [](Hast::Details::HastExpressionImpl* expr) { expr->BeginTranslationUnitProcessing(); });

        TraverseChildren(cont, node);
    }

private:
    template <typename Cont, typename L>
    static void ForEachExpression(Cont& cont, L l)
    {
        for (auto& item : cont) {
            l(item);
        }
    }

    static void TraverseChildren(std::list<Hast::Details::HastExpressionImpl*>& cont, const Node& node)
    {
        for (auto item : node.GetChildren()) {
            ForEachExpression(cont, [item](Hast::Details::HastExpressionImpl* expr) { expr->EnterChildNode(*item); });

            TraverseChildren(cont, *item);

            ForEachExpression(cont, [](Hast::Details::HastExpressionImpl* expr) { expr->LeaveChildNode(); });
        }
    }

private:
    std::list<std::unique_ptr<Hast::Details::HastExpressionImpl>> myExpressions;
    std::list<Hast::Details::HastExpressionImpl*> myPureCExpressions;
    std::list<Hast::Details::HastExpressionImpl*> myCppExpressions;
};

HastMatchingEngine::HastMatchingEngine() : myImpl(std::make_unique<Impl>()) {}

HastMatchingEngine::~HastMatchingEngine() {}

void HastMatchingEngine::DeleteExpression(const HastExpression* ptr)
{
    return myImpl->DeleteExpression(ptr);
}

HastExpressionPtr HastMatchingEngine::BuildExpression(const std::string_view expression, SupportedLanguages languages,
                                                      std::string* errorMsg)
{
    return myImpl->BuildExpression(expression, *this, languages, errorMsg);
}

void HastMatchingEngine::MatchAll(const TranslationUnit& node)
{
    return myImpl->MatchAll(node);
}

}  //  namespace HCXX
