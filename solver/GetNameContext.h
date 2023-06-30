/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md

#ifndef COODDY_GETNAMECONTEXT_H
#define COODDY_GETNAMECONTEXT_H

struct InstructionTreeNode {
    Instruction instruction;
    uint32_t depth;
};

using InstructionTree = std::vector<InstructionTreeNode>;

class GetNameContext {
    InstructionTree myInstructionTree;
    FunctionBehaviorImpl& myBehavior;
    const GetCalleeNameCb& myGetCalleeNameCb;
    const GetSourceInRangeCb& myGetSourceInRangeCb;
    uint32_t myCurrNode = -1;

public:
    explicit GetNameContext(InstructionTree tree, FunctionBehaviorImpl& behavior,
                            const GetCalleeNameCb& getCalleeNameCb, const GetSourceInRangeCb& getSourceInRangeCb)
        : myInstructionTree(std::move(tree)),
          myBehavior(behavior),
          myGetCalleeNameCb(getCalleeNameCb),
          myGetSourceInRangeCb(getSourceInRangeCb)
    {}

    std::string GetName()
    {
        if (++myCurrNode >= myInstructionTree.size()) {
            return {};  // LCOV_EXCL_LINE: broken AST
        }
        auto kind = myBehavior.GetInstructionKind(myInstructionTree[myCurrNode].instruction);
        return InstructionProcessor::GetProcessor(kind).GetName(*this);
    }

    std::string GetLastSiblingName()
    {
        if (++myCurrNode >= myInstructionTree.size()) {
            return {};  // LCOV_EXCL_LINE: broken AST
        }
        auto depth = myInstructionTree[myCurrNode].depth;
        auto lastSiblingNode = myCurrNode;
        for (; myCurrNode < myInstructionTree.size() && myInstructionTree[myCurrNode].depth >= depth; ++myCurrNode) {
            if (myInstructionTree[myCurrNode].depth == depth) {
                lastSiblingNode = myCurrNode;
            }
        }
        myCurrNode = lastSiblingNode;
        auto kind = myBehavior.GetInstructionKind(myInstructionTree[myCurrNode].instruction);
        return InstructionProcessor::GetProcessor(kind).GetName(*this);
    }

    Instruction GetCurInstruction()
    {
        return myInstructionTree[myCurrNode].instruction;
    }

    DataStream& GetInstructionsSet()
    {
        return myBehavior.GetInstructionsSet();
    }

    std::string GetSourceByRange()
    {
        return myGetSourceInRangeCb(myBehavior.GetSourceRange(myInstructionTree[myCurrNode].instruction));
    }

    std::string GetSourceByRange(SourceRange range)
    {
        return myGetSourceInRangeCb(range);
    }

    std::string GetCalleeName()
    {
        return myGetCalleeNameCb(myInstructionTree[myCurrNode].instruction);
    }
};

#endif  // COODDY_GETNAMECONTEXT_H
