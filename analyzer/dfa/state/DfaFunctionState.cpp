/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <ast/LoopStatement.h>
#include <dfa/DfaChecker.h>

namespace HCXX {

DfaState& DfaFunctionState::GetState(const Node* node)
{
    auto& state = myState[node];
    if (state == nullptr) {
        state = std::make_unique<DfaState>(*this, node);
    }
    return *state;
}

DfaState* DfaFunctionState::FindState(const Node* node) const
{
    auto state = myState.find(node);
    return state != myState.end() ? state->second.get() : nullptr;
}

void DfaFunctionState::JoinByAnd(const DfaFunctionState& other, Annotation::Kind annotation)
{
    for (auto& state : myState) {
        if (other.FindState(state.first) == nullptr) {
            state.second->ClearAnnotation(annotation);
        }
    }
    for (const auto& state : other.myState) {
        auto dstState = FindState(state.first);
        if (dstState == nullptr || !dstState->HasAnnotation(annotation)) {
            continue;
        }
        std::unordered_map<uint64_t, bool> offsets;
        for (const auto& it : state.second->GetAnnotationSources(annotation)) {
            offsets[it.first.GetMemoryOffset().GetRawValue()] = false;
        }

        auto dstSources = dstState->GetAnnotationSources(annotation);
        dstState->ClearAnnotation(annotation);

        for (const auto& it : dstSources) {
            auto offsetIt = offsets.find(it.first.GetMemoryOffset().GetRawValue());
            if (offsetIt != offsets.end()) {
                offsetIt->second = true;
                dstState->Annotate(it.first, *it.second);
            }
        }
        for (const auto& it : state.second->GetAnnotationSources(annotation)) {
            if (offsets[it.first.GetMemoryOffset().GetRawValue()]) {
                dstState->Annotate(it.first, *it.second);
            }
        }
    }
}

void DfaFunctionState::Join(DfaFunctionState& other, bool canModifySource)
{
    auto loop = Node::Cast<LoopStatement>(myBlock.GetTerminatorStatement());

    for (int i = 0; i < Annotation::GetAnnotationsCount(); i++) {
        if (loop != nullptr && !Annotation::IsPropagatedThroughLoop(i) &&
            loop->GetLoopKind() != LoopStatement::LoopKind::DO_WHILE) {
            continue;
        }
        if (!Annotation::IsPropagatedToSubScopes(i)) {
            if (GetCfgBlock().GetPos() != other.GetCfgBlock().GetPredecessors().back()) {
                continue;
            }
        }
        if (myJoinCount != 0 && loop == nullptr && Annotation::IsJoinedByAnd(i)) {
            JoinByAnd(other, i);
            continue;
        }
        for (const auto& it : other.myState) {
            if (it.first == nullptr || !it.first->IsDeclaration() || !it.second->HasAnnotation(i)) {
                continue;
            }
            it.second->MergeAnnotation(i, GetState(it.first), canModifySource);
        }
    }
    myJoinCount++;
}

};  // namespace HCXX
