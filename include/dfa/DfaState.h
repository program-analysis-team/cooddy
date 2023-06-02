/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_INCLUDE_DFA_DFASTATE_H_
#define COODDY_ANALYZER_INCLUDE_DFA_DFASTATE_H_

#include <ast/Node.h>
#include <dfa/Annotation.h>
#include <dfa/DfaFunctionState.h>
#include <dfa/SuspiciousPath.h>

#include <algorithm>
#include <array>
#include <unordered_set>
#include <vector>

constexpr uint64_t KEEP_USER_DATA = UINT64_MAX;

namespace HCXX {

class RefExpression;

/**
 * @brief A collection of all information collected for a particular AST Node.
 *
 * Each DfaState points to a single AST node and contains information about it. Generally only one state exists for
 * each AST node, but this is not a rule. DfaStates are grouped by CFG blocks, but different CFG blocks can have
 * additional states that point to variable declarations outside of those blocks.
 *
 * The information stored includes, mainly, annotations and various pointers for AST nodes where variables are being
 * declared and used. See member variables for more information.
 */
class DfaState {
public:
    struct AnnotationHash {
        std::size_t operator()(const Annotation& a) const noexcept
        {
            return a.GetInstruction() ^ a.GetMemoryOffset().GetRawValue();
        }
    };
    using AnnotationSources = std::unordered_map<Annotation, const Node*, AnnotationHash>;
    using AnnotationsArray = std::array<AnnotationSources, Annotation::MAX_ANNOTATIONS_COUNT>;
    using SubStates = std::unordered_map<uint64_t, std::unique_ptr<DfaState>>;

    DfaState(DfaFunctionState& funcState, const Node* node, VirtualOffset subOffset = VirtualOffset());
    ~DfaState() = default;

    DfaFunctionState& GetFuncState() const
    {
        return myFuncState;
    }

    DfaState& GetSubState(VirtualOffset memOffset);

    const Node* GetNode() const
    {
        return myNode;
    }

    DfaState* GetDeclState() const
    {
        // COODDY_SUPPRESS ReturnMemberAddressChecker
        return myDeclState;
    }

    template <class NodeT>
    const NodeT* GetNodeAs() const
    {
        return Node::Cast<NodeT>(myNode);
    }

    void SetParentState(DfaState* parentState)
    {
        myParentState = parentState;
    }

    DfaState* GetParentState() const
    {
        // COODDY_SUPPRESS ReturnMemberAddressChecker
        return myParentState;
    }

    const DfaState* GetTopState() const
    {
        return GetParentState() == nullptr ? this : GetParentState()->GetTopState();
    }

    template <class NodeT>
    const NodeT* GetParentAs() const
    {
        return myParentState == nullptr ? nullptr : Node::Cast<NodeT>(myParentState->myNode);
    }

    template <class NodeT>
    const NodeT* GetParentOfType() const
    {
        auto parent = GetParentAs<NodeT>();
        if (parent != nullptr) {
            return parent;
        }
        if (GetParentState() != nullptr) {
            return GetParentState()->GetParentOfType<NodeT>();
        }
        return nullptr;
    }

    template <class TUserData = uint64_t>
    void Annotate(Annotation::Kind kind, TUserData userData = 0, bool replaceExisting = false)
    {
        if (replaceExisting) {
            myAnnotationSources[kind] = AnnotationSources();
            if (myDeclState != nullptr) {
                myDeclState->MoveDeclAnnotation(kind, nullptr, false);
            }
        }
        Annotation annotation{kind, Annotation::ArgInfo(), GetInstruction(), myMemoryOffset};
        annotation.GetUserData<TUserData>() = userData;
        Annotate(std::move(annotation), *myNode);
    }

    void Annotate(const Annotation& annotation, const Node& source)
    {
        myAnnotationSources[annotation.GetKind()].emplace(annotation, &source);
        if (myDeclState != nullptr) {
            myDeclState->Annotate(Annotation(annotation, myMemoryOffset), source);
        }
    }

    template <class TUserData = uint64_t>
    void CloneAnnotation(Annotation::Kind source, Annotation::Kind destination, TUserData userData = 0)
    {
        for (auto& it : GetAnnotationSources(source)) {
            Annotation annotation{destination, it.first.GetArgInfo(), it.first.GetInstruction(),
                                  it.first.GetMemoryOffset()};
            if (userData != KEEP_USER_DATA) {
                annotation.GetUserData<TUserData>() = userData;
            } else {
                annotation.GetUserData<TUserData>() = it.first.GetUserData<TUserData>();
            }
            Annotate(annotation, *it.second);
        }
    }

    void MergeAnnotation(Annotation::Kind annotation, DfaState& destination, bool canExtract);

    void CopyAnnotation(Annotation::Kind annotation, DfaState& destination, bool copyDeclAnnotations = false)
    {
        if (copyDeclAnnotations) {
            CopyDeclAnnotations();
        }
        for (auto& it : GetAnnotationSources(annotation)) {
            destination.Annotate(it.first, *it.second);
        }
    }

    void MoveAnnotation(Annotation::Kind annotation, DfaState& destination, bool moveInDecl = true)
    {
        if (HasAnnotation(annotation)) {
            CopyAnnotation(annotation, destination);
            if (moveInDecl) {
                MoveDeclAnnotation(annotation, &destination);
            }
            ClearAnnotation(annotation, false);
        }
    }

    bool HasAnnotation(Annotation::Kind annotation) const
    {
        return !myAnnotationSources[annotation].empty();
    }

    /// returns true if there is an annotation with specified kind and offset
    bool HasAnnotation(Annotation::Kind annotation, VirtualOffset offset) const;

    void ClearAnnotation(Annotation::Kind annotation, bool clearInDecl = true)
    {
        myAnnotationSources[annotation] = AnnotationSources();
        if (clearInDecl) {
            MoveDeclAnnotation(annotation, nullptr);
        }
    }

    /***
     * @return true if DeclState has annotation, which should be passed to inner memory.
     * For example: If annotation Write is set to structure, than all members of structure should have this annotation.
     */
    bool IsDeclHasNestedAnnotation(Annotation::Kind kind);
    /***
     * @return annotations of DeclState with relative offset on member of structure.
     * For example: This list of annotations is used to update suspiciousPath sink annotations when one annotation
     * is set to a structure (ZeroMemory) and influence on members and the second Deref annotations is set to the
     * member of structure.
     */
    std::vector<Annotation> GetDeclNestedAnnotations(Annotation::Kind kind, VirtualOffset baseOffset);

    const AnnotationSources& GetAnnotationSources(Annotation::Kind annotation) const
    {
        return myAnnotationSources[static_cast<int>(annotation)];
    }

    bool HasSameAnnotationSource(Annotation::Kind annotation1, Annotation::Kind annotation2, bool ignoreLeaves = false)
    {
        auto& sources1 = myAnnotationSources[static_cast<int>(annotation1)];
        auto& sources2 = myAnnotationSources[static_cast<int>(annotation2)];
        auto instruction = sources1.begin()->first.GetInstruction();
        auto pred = [&instruction](auto& p) { return p.first.GetInstruction() != instruction; };
        auto same = std::find_if(sources1.begin(), sources1.end(), pred) == sources1.end() &&
                    std::find_if(sources2.begin(), sources2.end(), pred) == sources2.end();
        return same && !(ignoreLeaves && sources1.begin()->first.IsSourceRange());
    }

    void AddSuspiciousPath(SuspiciousPath&& path);

    uint32_t GetInstruction() const
    {
        return myFuncState.GetContext().GetInstruction(*myNode);
    }

    const SubStates& GetSubStates() const
    {
        return mySubStates;
    }

    void InitSubStates();
    void ClearSubAnnotations();
    void CopySubOffsetAnnotations(Annotation::Kind annotation, DfaState* destination);

    void MoveDeclAnnotation(Annotation::Kind annotation, DfaState* destination, bool moveSubOffsets = true,
                            bool moveFromSameSource = true);
    void CopyDeclAnnotations();

    VirtualOffset GetMemoryOffset() const
    {
        return myMemoryOffset;
    }

    std::optional<Annotation> GetLeafAnnotation(Annotation::Kind kind);

    void DumpStateAnnotations();

private:
    // Maximum annotations count to be copied from declaration state
    static const uint64_t MAX_DECL_ANNOTATIONS = 16UL;

    void InitDeclState();

    /**
     * @brief DfaFunctionState for the CFG block this AST node/state belongs to.
     * @see DfaFunctionState class doc for more info.
     */
    DfaFunctionState& myFuncState;

    /**
     * @brief State for the variable declaration related to this variable state.
     *
     * For DfaStates that bind to an AST node where a variable is declared or used, myDeclState points to a DfaState
     * where the variable was declared. For a variable declaration, myDeclState points to this. For DfaStates not
     * related to declarations, myDeclState is nullptr.
     */
    DfaState* myDeclState = nullptr;

    /**
     * @brief State of this AST nodes's parent AST node.
     *
     * Generally this pointer is only valid when this particular DfaState is undergoing analysis, for example during a
     * CheckState call where this was the argument.
     */
    DfaState* myParentState = nullptr;

    /**
     * @brief When applicable, holds the offset for the operation performed on a variable.
     *
     * For example, `myMemoryOffset` will have a valid value denoting type of operation when pointing to an AST node
     * `&x`, `x.var`, etc.
     */
    VirtualOffset myMemoryOffset;

    /// Pointer to the AST node this state is for.
    const Node* myNode = nullptr;

    /**
     * @brief Storage for all different annotations for the variable, related to this DfaState.
     *
     * When this DfaState is bound to an AST node for a variable declaration or usage, myAnnotationSources will hold all
     * annotations bound to this variable, as well as AST nodes from which the annotation was propagated.
     *
     * AnnotationsArray is an array of unordered maps, where each array index corresponds to an annotation kind, and
     * each unordered_map maps annotations of such kind to an AST node.
     */
    AnnotationsArray myAnnotationSources;

    SubStates mySubStates;
    bool myDeclAnnotationsCopied = false;
};

}  // namespace HCXX

#endif  // COODDY_ANALYZER_INCLUDE_DFA_DFASTATE_H_
