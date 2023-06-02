/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <ast/InitListExpression.h>
#include <ast/UnaryExpression.h>
#include <ast/VarDecl.h>
#include <dfa/DfaChecker.h>
#include <utils/Log.h>

namespace HCXX {

DfaState::DfaState(DfaFunctionState& funcState, const Node* node, VirtualOffset subOffset)
    : myFuncState(funcState), myNode(node)
{
    if (myNode != nullptr) {
        InitDeclState();
    }
    if (!subOffset.IsSourceObject()) {
        myMemoryOffset += subOffset;
    } else {
        CopyDeclAnnotations();
    }
}

void DfaState::InitDeclState()
{
    const Node* declNode = nullptr;
    auto typedNode = GetNodeAs<TypedNode>();
    if (typedNode != nullptr) {
        auto offset = typedNode->GetOffsetInDeclaration();
        if (offset != std::nullopt) {
            myMemoryOffset += offset.value();
        }
        auto decl = typedNode->GetDeclaration();
        if (decl != nullptr) {
            declNode = decl;
        }
    }
    myDeclState = (declNode != nullptr) ? &GetFuncState().GetState(declNode) : nullptr;
}

void DfaState::InitSubStates()
{
    if (myDeclState == nullptr) {
        return;
    }
    for (int i = 0; i < Annotation::GetAnnotationsCount(); i++) {
        Annotation::Kind annotation = static_cast<Annotation::Kind>(i);
        if (!Annotation::IsPropagatedByDecl(annotation)) {
            continue;
        }
        for (const auto& it : myDeclState->GetAnnotationSources(annotation)) {
            auto subIt = mySubStates.find(it.first.GetMemoryOffset().GetRawValue());
            if (subIt != mySubStates.end()) {
                subIt->second->myAnnotationSources[i].emplace(it.first, it.second);
            }
        }
    }
}

/**
 * Copy annotations from declaration's state to current state
 */
void DfaState::CopyDeclAnnotations()
{
    if (myDeclState == nullptr || myDeclAnnotationsCopied) {
        return;
    }
    for (int i = 0; i < Annotation::GetAnnotationsCount(); i++) {
        Annotation::Kind annotation = static_cast<Annotation::Kind>(i);
        if (!Annotation::IsPropagatedByDecl(annotation)) {
            continue;
        }
        uint32_t copied = 0;
        for (const auto& it : myDeclState->GetAnnotationSources(annotation)) {
            if (it.first.GetMemoryOffset() == myMemoryOffset || Annotation::IsPropagatedToFields(annotation)) {
                myAnnotationSources[i].emplace(it.first, it.second);
                if (++copied > MAX_DECL_ANNOTATIONS) {
                    break;
                }
            }
        }
    }
    myDeclAnnotationsCopied = true;
}

void DfaState::MergeAnnotation(Annotation::Kind annotation, DfaState& destination, bool canExtract)
{
    auto& src = myAnnotationSources[annotation];
    auto& dst = destination.myAnnotationSources[annotation];
    if (src.size() < MAX_DECL_ANNOTATIONS * MAX_DECL_ANNOTATIONS) {
        if (canExtract) {
            dst.merge(src);
        } else {
            dst.insert(src.begin(), src.end());
        }
    } else {
        // LCOV_EXCL_START
        std::unordered_map<uint64_t, uint32_t> offsetCount;
        for (auto& it : src) {
            if (++offsetCount[it.first.GetMemoryOffset().GetRawValue()] <= MAX_DECL_ANNOTATIONS) {
                dst.emplace(it.first, it.second);
            }
        }
        // LCOV_EXCL_STOP
    }
}

void DfaState::CopySubOffsetAnnotations(Annotation::Kind annotation, DfaState* destination)
{
    if (myDeclState == nullptr || destination == nullptr || destination->myDeclState == nullptr) {
        return;
    }

    for (auto& it : myDeclState->myAnnotationSources[static_cast<int>(annotation)]) {
        auto offset = it.first.GetMemoryOffset();
        if (!offset.IsSubOffsetOf(myMemoryOffset) || offset == myMemoryOffset) {
            continue;
        }

        auto destOffset = (offset - myMemoryOffset) + destination->myMemoryOffset;
        destination->myDeclState->Annotate(Annotation(it.first, destOffset), *it.second);
    }
}

void DfaState::MoveDeclAnnotation(Annotation::Kind annotation, DfaState* destination, bool moveSubOffsets,
                                  bool moveFromSameSource)
{
    if (myDeclState == nullptr) {
        return;
    }
    auto& annoSource = myDeclState->myAnnotationSources[static_cast<int>(annotation)];
    for (auto it = annoSource.begin(); it != annoSource.end();) {
        auto offset = it->first.GetMemoryOffset();
        if (!(moveSubOffsets ? offset.IsSubOffsetOf(myMemoryOffset) : offset == myMemoryOffset)) {
            ++it;
            continue;
        }
        if (!moveFromSameSource && it->second->GetInnerNode() == myNode->GetInnerNode() && offset == myMemoryOffset) {
            ++it;
            continue;
        }
        if (destination != nullptr && destination->myDeclState != nullptr) {
            auto destOffset = (offset - myMemoryOffset) + destination->myMemoryOffset;
            destination->myDeclState->Annotate(Annotation(it->first, destOffset), *it->second);
        }
        auto toErase = it++;
        annoSource.erase(toErase);
    }
}
std::vector<Annotation> DfaState::GetDeclNestedAnnotations(Annotation::Kind kind, VirtualOffset baseOffset)
{
    if (myDeclState == nullptr) {
        return {};
    }
    auto& sources = myDeclState->myAnnotationSources[static_cast<int>(kind)];
    std::vector<Annotation> res;
    for (auto& source : sources) {
        auto relativeOffset = source.first.GetMemoryOffset() - baseOffset;
        if (relativeOffset.ExtractSubOffset().first == VirtualOffset::Kind::INDEX && relativeOffset.IsSourceObject()) {
            res.emplace_back(source.first);
        }
    }

    return res;
}

bool DfaState::IsDeclHasNestedAnnotation(Annotation::Kind kind)
{
    if (myDeclState == nullptr) {
        return false;
    }

    auto& sources = myDeclState->myAnnotationSources[static_cast<int>(kind)];
    for (auto& source : sources) {
        if (myNode != source.second && (myMemoryOffset + VirtualOffset(VirtualOffset::Kind::DEREF))
                                           .IsSubOffsetOf(source.first.GetMemoryOffset())) {
            return true;
        }
    }

    return false;
}

void DfaState::ClearSubAnnotations()
{
    for (int i = 0; i < Annotation::GetAnnotationsCount(); i++) {
        auto& sources = myAnnotationSources[i];
        for (auto it = sources.begin(); it != sources.end();) {
            if (!it->first.IsSourceObject()) {
                auto toErase = it++;
                sources.erase(toErase);
            } else {
                ++it;
            }
        }
    }
}

DfaState& DfaState::GetSubState(VirtualOffset memOffset)
{
    if (memOffset.IsSourceObject()) {
        return *this;
    }

    auto initListExpr = Node::Cast<InitListExpression>(myNode);
    if (initListExpr != nullptr) {
        auto offset = memOffset;
        auto idxDesc = offset.ExtractSubOffset();
        auto& initList = initListExpr->GetInitList();

        if (idxDesc.first == VirtualOffset::Kind::INDEX && idxDesc.second < initList.size()) {
            auto& initState = myFuncState.GetState(initList[idxDesc.second]);
            return initState.GetSubState(offset);
        }
    }

    auto& subState = mySubStates[(myMemoryOffset + memOffset).GetRawValue()];
    if (subState == nullptr) {
        subState = std::make_unique<DfaState>(myFuncState, myNode, memOffset);
        subState->SetParentState(myParentState);
    }
    return *subState;
}

void DfaState::AddSuspiciousPath(SuspiciousPath&& path)
{
    if (path.sinkAnnotations.empty()) {
        for (auto& it : myAnnotationSources[static_cast<int>(path.sinkKind)]) {
            if (!path.sourceOffset || *path.sourceOffset == it.first.GetMemoryOffset()) {
                path.sinkAnnotations.emplace_back(it.first);
            }
        }
    }
    if (path.sourceKind != 0) {
        for (auto& it : myAnnotationSources[static_cast<int>(path.sourceKind)]) {
            path.sourceAnnotations.emplace_back(it.first);
        }
    } else if (path.sourceAnnotations.empty()) {
        path.sourceAnnotations.emplace_back(Annotation{0, Annotation::ArgInfo(), GetInstruction(),
                                                       path.sourceOffset ? *path.sourceOffset : VirtualOffset()});
    }
    if (Annotation::IsUntrustedSink(path.sinkKind)) {
        path.checkPathFlags |= CheckPathParams::USE_UNTRUSTED_SOURCES;
    }
    path.jointInstruction = GetInstruction();
    myFuncState.GetContext().AddSuspiciousPath(std::move(path));
}

std::optional<Annotation> DfaState::GetLeafAnnotation(Annotation::Kind kind)
{
    for (auto& annotationNodeMap : this->GetAnnotationSources(kind)) {
        if (annotationNodeMap.first.IsSourceRange() && this->GetNode() == annotationNodeMap.second) {
            return annotationNodeMap.first;
        }
    }
    return std::nullopt;
}

bool DfaState::HasAnnotation(Annotation::Kind annotation, VirtualOffset offset) const
{
    for (auto& [anno, _] : myAnnotationSources[annotation]) {
        if (anno.GetMemoryOffset() == offset) {
            return true;
        }
    }
    return false;
}

// LCOV_EXCL_START
void DfaState::DumpStateAnnotations()
{
    HCXX::Log(HCXX::LogLevel::DEBUG) << "------------------------------" << std::endl;
    HCXX::Log(HCXX::LogLevel::DEBUG) << "------ START DUMP STATE ------" << std::endl;
    HCXX::Log(HCXX::LogLevel::DEBUG) << "||| CURRENT NODE -->>" << std::endl;
    std::cout << this->myNode->Dump();
    HCXX::Log(HCXX::LogLevel::DEBUG) << "<<-- CURRENT NODE |||" << std::endl;
    uint32_t annotationsCount = Annotation::GetAnnotationsCount();
    for (uint32_t kind = 0; kind < annotationsCount; kind++) {
        for (auto annotationNodeMap : this->GetAnnotationSources(kind)) {
            HCXX::Log(HCXX::LogLevel::DEBUG)
                << "*** STATE HAS ANNOTATION: " << annotationNodeMap.first.GetKindName(kind) << " ***" << std::endl;
            HCXX::Log(HCXX::LogLevel::DEBUG) << "+++ ANNOTATION SOURCE NODE -->>" << std::endl;
            std::cout << annotationNodeMap.second->Dump();
            HCXX::Log(HCXX::LogLevel::DEBUG) << "<<-- ANNOTATION SOURCE NODE +++" << std::endl;
        }
    }
    HCXX::Log(HCXX::LogLevel::DEBUG) << "----- END DUMP STATE -----" << std::endl;
    HCXX::Log(HCXX::LogLevel::DEBUG) << "--------------------------" << std::endl;
}
// LCOV_EXCL_STOP

};  // namespace HCXX
