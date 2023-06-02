/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <ast/BinaryExpression.h>
#include <ast/CxxNewExpression.h>
#include <ast/CxxRecordDecl.h>
#include <ast/InitListExpression.h>
#include <ast/MemberExpression.h>
#include <ast/VarDecl.h>
#include <dfa/TaintedChecker.h>
using namespace HCXX;

class TypeSizeMismatchChecker : public DfaChecker {
    Annotation::Kind myAllocKind;
    Annotation::Kind myFieldAccessKind;
    Annotation::Kind myStackAllocKind = 0;

public:
    TypeSizeMismatchChecker()
    {
        myAllocKind = Annotation::GetKind("AllocSource");
        myFieldAccessKind = Annotation::RegisterKind("FieldAccess", Annotation::PROPAGATED_BY_DECL);
    }

    uint32_t GetMaxFieldOffset(DfaState& state)
    {
        uint32_t maxFieldOffset = 0;
        for (auto& it : state.GetAnnotationSources(myFieldAccessKind)) {
            auto offset = it.first.GetUserData<uint32_t>();
            maxFieldOffset = std::max(maxFieldOffset, offset);
        }
        return maxFieldOffset;
    }

    void AnnotateFieldOffset(DfaState& state, const MemberExpression& memberExpr)
    {
        auto base = memberExpr.GetBase();
        auto field = Node::Cast<FieldDecl>(memberExpr.GetValue());

        if (base != nullptr && field != nullptr) {
            uint32_t sizeOfType = field->GetTranslationUnit()->GetSizeOfType(field->GetType());
            uint32_t offset = (field->GetOffset() + sizeOfType) / 8;
            auto& baseState = state.GetFuncState().GetState(base);
            if (offset > GetMaxFieldOffset(baseState)) {
                baseState.Annotate(myFieldAccessKind, offset, true);
            }
        }
    }

    void InitState(DfaState& state) override
    {
        auto memberExpr = state.GetNodeAs<MemberExpression>();
        if (memberExpr != nullptr) {
            AnnotateFieldOffset(state, *memberExpr);
        }
    }

    void CheckGlobalVar(DfaState& state, ProblemsHolder& holder) override
    {
        CheckState(state, holder);
    }

    void CheckDeclaration(DfaState& state, ProblemsHolder& holder) override
    {
        auto varDecl = state.GetNodeAs<VarDecl>();
        if (varDecl == nullptr) {
            return;
        }
        int32_t size = 0;
        if (Node::Cast<RecordDecl>(varDecl->GetType().GetDeclaration()) != nullptr) {
            size = varDecl->GetType().GetSizeInBits() / 8;
        } else if (auto arrSize = varDecl->GetNumOfElementsInArray();
                   varDecl->GetType().GetSizeInBits() / 8 == arrSize) {
            size = arrSize;
        } else if (arrSize != 0) {
            size = varDecl->GetType().GetSizeInBits() / 8 / arrSize;
        }
        if (size == 0) {
            return;
        }
        SuspiciousPath path{*this,
                            myFieldAccessKind,
                            myStackAllocKind,
                            StrLocales::GetStringLocale("STACK_VARIABLE"),
                            {Condition::Operation::NONE, size}};

        FillSinks(state, path);
        for (auto& offset : {VirtualOffset(), VirtualOffset(VirtualOffset::Kind::ADDR_OF)}) {
            Annotation sourceAnnotation{myStackAllocKind, Annotation::ArgInfo(), state.GetInstruction(), offset};
            path.sourceAnnotations.emplace_back(sourceAnnotation);
        }
        state.AddSuspiciousPath(std::move(path));
    }

    void CheckState(DfaState& state, ProblemsHolder& holder) override
    {
        auto varDecl = state.GetNodeAs<VarDecl>();
        auto type = varDecl != nullptr ? varDecl->GetType() : Type();

        if ((Node::Cast<RecordDecl>(type.GetDeclaration()) == nullptr && !type.IsConstantArray() || type.IsPointer()) &&
            !state.HasAnnotation(myAllocKind)) {
            return;
        }

        if (state.HasAnnotation(myFieldAccessKind)) {
            int32_t typeSize = GetTypeSizeInBits(state) / 8;
            if (typeSize == 0) {
                return;
            }
            SuspiciousPath path{
                *this, myFieldAccessKind, myAllocKind, "BOO.TYPESIZE.MISMATCH", {Condition::Operation::NONE, typeSize}};

            state.AddSuspiciousPath(std::move(path));
        }
    }

    bool OnSourceExecuted(const SourceExecInfo& sourceInfo) override
    {
        auto offset = sourceInfo.annotation.GetMemoryOffset();
        if (offset == VirtualOffset(VirtualOffset::Kind::ADDR_OF)) {
            sourceInfo.context.AddOffset(sourceInfo.exprId, offset);
        }
        return true;
    }

    bool OnSinkExecuted(const SinkExecInfo& sinkInfo) override
    {
        if (sinkInfo.annotation.GetUserData<uint32_t>() > sinkInfo.path.sinkCondition.operand) {
            return true;
        } else {
            return false;
        }
    }

    /// Here we check for malloc and new allocated memory. We have 1 source and may have multiple sinks
    int32_t GetTypeSizeInBits(DfaState& state)
    {
        for (auto& it : state.GetAnnotationSources(myAllocKind)) {
            auto newExp = Node::Cast<CxxNewExpression>(it.second);
            if (newExp != nullptr) {
                if (newExp->GetType().IsTrivial() && newExp->GetType().GetSizeInBits() == 8) {
                    auto size = newExp->GetNumOfElementsInArray();
                    return (size > 0 ? size * 8 : 8);
                }
                return newExp->GetType().GetSizeInBits();
            } else {
                // malloc
                // TODO: [HCAT-1939] - support malloc
                return 0;
            }
        }
        return 0;
    }

    void FillSinks(DfaState& state, SuspiciousPath& path)
    {
        for (auto& it : state.GetAnnotationSources(myFieldAccessKind)) {
            auto offset = it.first.GetMemoryOffset();
            if (offset.IsSourceObject() || offset == VirtualOffset(VirtualOffset::Kind::ADDR_OF)) {
                path.sinkAnnotations.emplace_back(it.first);
            }
        }
    }
};

std::unique_ptr<HCXX::Checker> CreateTypeSizeMismatchChecker()
{
    return std::make_unique<TypeSizeMismatchChecker>();
}
