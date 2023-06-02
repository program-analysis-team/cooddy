/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_SOURCE_SOLVER_PROCESSORS_INITLISTINSTRUCTION_H_
#define COODDY_ANALYZER_SOURCE_SOLVER_PROCESSORS_INITLISTINSTRUCTION_H_

#include <ast/InitListExpression.h>

namespace Processor {

class InitListInstruction : public InstructionProcessor {
    void Compile(const Node& node, CompileContext& context) override
    {
        auto* list = Node::Cast<InitListExpression>(&node);
        if (list == nullptr) {
            return;  // LCOV_EXCL_LINE
        }
        auto& initList = list->GetInitList();
        context.Add<bool>(list->IsForArray());
        uint8_t initListSize = std::min(initList.size(), size_t(UINT8_MAX));
        context.Add<uint8_t>(initListSize);
        int64_t maxVal = 0;
        for (auto i = 0; i < initListSize; ++i) {
            context.Compile(initList[i]);
            if (auto val = Node::Cast<TypedNode>(initList[i]);
                val != nullptr && val->GetType().IsIntegralType() && val->IsConstExpr()) {
                maxVal = std::max(maxVal, val->GetSLimitedValue());
            }
        }
        context.Add<int64_t>(maxVal);
    }
    z3::expr Execute(ExecutionContext& context, SymbolId& symbolId) override
    {
        auto result = context->CreateSymbolExpr(symbolId);
        bool isForArray = context.Get<bool>();
        uint32_t initsCount = context.Get<uint8_t>();
        for (uint32_t i = 0; i < initsCount; ++i) {
            SymbolId initSymbolId = symbolId;
            initSymbolId.AddSubOffset(
                VirtualOffset((!isForArray || i != 0) ? VirtualOffset::Kind::INDEX : VirtualOffset::Kind::DEREF, i));
            auto& initExpr = context->SetSymbol(initSymbolId, context.Execute(nullptr), &initSymbolId);
            context->CopySubOffsetUntrustedSource(result, initExpr);
        }
        auto maxVal = context.Get<int64_t>();
        if (maxVal != 0) {
            context->AddArrayMaxValue(result, maxVal);
        }
        return result;
    }
};

};  // namespace Processor

#endif  // COODDY_ANALYZER_SOURCE_SOLVER_PROCESSORS_INITLISTINSTRUCTION_H_
