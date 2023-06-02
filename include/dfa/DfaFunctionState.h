/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_INCLUDE_DFA_DFAFUNCTIONSTATE_H_
#define COODDY_ANALYZER_INCLUDE_DFA_DFAFUNCTIONSTATE_H_

#include <ast/Node.h>
#include <cfg/CfgBlock.h>
#include <dfa/DfaFunctionContext.h>

#include <functional>
#include <memory>
#include <unordered_map>

namespace HCXX {

class DfaState;
class FunctionDecl;

/**
 * @brief A collection of properties collected from each CFG block and joined (merged) during analysis.
 *
 * Instances of DfaFunctionState get created for each CFG block, analyzed, then joined together, until a full state for
 * the whole function is formed.
 */
class DfaFunctionState {
public:
    explicit DfaFunctionState(const DfaFunctionContext& context, const CfgBlock& block)
        : myContext(context), myBlock(block)
    {}
    ~DfaFunctionState() = default;

    const DfaFunctionContext& GetContext() const
    {
        return myContext;
    }

    const CfgBlock& GetCfgBlock() const
    {
        return myBlock;
    }

    // get or create (if not exist) state for the specific node
    virtual DfaState& GetState(const Node* node);

    // find state for the specific node (nullptr will be returned if there is no state for node)
    virtual DfaState* FindState(const Node* node) const;

    // join all data form the 'other' memory to this memory instance
    void Join(DfaFunctionState& other, bool canModifySource = false);

    void VisitStates(const std::function<void(DfaState&)>& callback)
    {
        for (auto& it : myState) {
            callback(*it.second);
        }
    }

private:
    void JoinByAnd(const DfaFunctionState& other, Annotation::Kind annotation);

    const DfaFunctionContext& myContext;
    const CfgBlock& myBlock;
    std::unordered_map<const HCXX::Node*, std::unique_ptr<HCXX::DfaState>> myState;
    uint32_t myJoinCount = 0;
};

}  // namespace HCXX

#endif  // COODDY_ANALYZER_INCLUDE_DFA_DFAFUNCTIONSTATE_H_
