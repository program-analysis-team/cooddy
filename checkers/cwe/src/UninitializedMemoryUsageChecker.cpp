/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md

#include <ast/CatchExpression.h>
#include <ast/CxxConstructExpression.h>
#include <ast/CxxRecordDecl.h>
#include <ast/VarDecl.h>
#include <dfa/AllocSourceChecker.h>
#include <dfa/DfaChecker.h>
#include <utils/LocaleUtils.h>

using namespace HCXX;

class UninitializedMemoryUsageChecker : public DfaChecker {
    Annotation::Kind myReadKind;
    Annotation::Kind myAllocSourceKind;
    Annotation::Kind myInitMemKind;
    Annotation::Kind myInitMemKindDecl;
    Annotation::Kind myZeroMemoryKind;

public:
    bool Initialize(const Checker::Config& config) override
    {
        myReadKind = Annotation::GetKind("Read");
        myInitMemKind = Annotation::GetKind("InitMemory");
        myInitMemKindDecl = Annotation::RegisterKind("InitMemoryDecl", Annotation::Trait::PROPAGATED_BY_DECL);
        myZeroMemoryKind = Annotation::GetKind("ZeroMemory");
        myAllocSourceKind = Annotation::GetKind("AllocSource");
        return true;
    }

    void InitState(DfaState& state) override
    {
        if (state.HasAnnotation(myInitMemKind) && !state.HasAnnotation(myInitMemKindDecl)) {
            state.Annotate(myInitMemKindDecl);
        }
    }

    bool IsDefaultInitializer(const VarDecl& varDecl)
    {
        auto initializer = varDecl.GetInitializer();
        if (initializer == nullptr) {
            return true;
        }
        auto constructCall = Node::Cast<CxxConstructExpression>(initializer);
        if (constructCall == nullptr || constructCall->GetFunction() == nullptr) {
            return false;
        }
        auto ctor = Node::Cast<CxxCtorDecl>(constructCall->GetFunction());
        return ctor != nullptr && ctor->IsDefault() && ctor->IsDefaulted();
    }

    bool SkipState(DfaState& state)
    {
        if (!state.HasAnnotation(myAllocSourceKind) || state.HasAnnotation(myZeroMemoryKind) ||
            state.HasAnnotation(myInitMemKindDecl)) {
            return true;
        }
        for (auto& [anno, node] : state.GetAnnotationSources(myAllocSourceKind)) {
            auto allocInfo = anno.GetUserData<AllocUserData>();
            if (allocInfo.allocType != AllocUserData::ALLOC_FUNC) {
                return true;
            }
        }

        return false;
    }

    void CheckState(DfaState& state, ProblemsHolder& holder) override
    {
        if (!SkipState(state) && state.GetDeclState() != nullptr) {
            SuspiciousPath path{*this, myReadKind, myAllocSourceKind,
                                StrLocales::GetStringLocale("UNINITIALIZED_MEMORY")};

            path.problemKind = "UNINIT.MEM";
            FillSinks(*state.GetDeclState(), path, nullptr, state, true);
            if (!path.sinkAnnotations.empty()) {
                state.AddSuspiciousPath(std::move(path));
            }
        }
    }

    void CheckDeclaration(DfaState& state, ProblemsHolder& holder) override
    {
        auto* varDecl = state.GetNodeAs<VarDecl>();
        if (varDecl == nullptr || IsUnsupportedVar(*varDecl)) {
            return;
        }
        if (varDecl->GetTranslationUnit()->HasParseErrors()) {
            return;  // LCOV_EXCL_LINE: HCAT-2779
        }
        if (IsDefaultInitializer(*varDecl) && state.HasAnnotation(myReadKind)) {
            if (state.GetParentOfType<CatchExpression>() != nullptr) {
                return;  // LCOV_EXCL_LINE
            }
            SuspiciousPath path{*this,
                                myReadKind,
                                0,
                                StrLocales::GetStringLocale("UNINITIALIZED_MEMORY"),
                                {Condition::Operation::EQ, UNDEFINED_VALUE}};

            auto sourceNode = varDecl->GetInitializer() != nullptr ? varDecl->GetInitializer() : varDecl;
            path.sourceAnnotations.emplace_back(
                Annotation{0, Annotation::ArgInfo(), state.GetFuncState().GetContext().GetInstruction(*sourceNode)});

            FillSinks(state, path, varDecl, state);
            if (!path.sinkAnnotations.empty()) {
                state.AddSuspiciousPath(std::move(path));
            }
        }
    }

    bool OnSinkExecuted(const SinkExecInfo& sinkInfo) override
    {
        auto offset = sinkInfo.annotation.GetMemoryOffset();
        if (offset.ExtractSubOffset().first == VirtualOffset::Kind::INDEX) {
            auto sinkExprId = sinkInfo.exprId;
            return sinkInfo.context.MakeFieldCondition(sinkInfo.sources, sinkExprId,
                                                       sinkInfo.annotation.GetMemoryOffset(), true);
        }
        return true;
    }

    bool OnReportProblem(ProblemInfo& problemInfo) override
    {
        if (auto funcCtx = problemInfo.trace.back().funcCtx; funcCtx != nullptr) {
            problemInfo.replacements.push_back(
                funcCtx->GetVarName(problemInfo.trace.back().annotation.GetInstruction()));
        }
        return true;
    }

    void CheckTerminator(const Node& terminator, DfaFunctionState& state, ProblemsHolder& holder) override
    {
        if (!terminator.IsKindOf(Node::Kind::GOTO_STATEMENT)) {
            return;
        }
        state.VisitStates([&](DfaState& it) {
            auto varDecl = it.GetNodeAs<VarDecl>();
            if (!it.HasAnnotation(myReadKind) || varDecl == nullptr || IsUnsupportedVar(*varDecl) ||
                varDecl->GetType().IsDeclaration()) {
                return;
            }
            if (!varDecl->GetRange().IsValid() || varDecl->GetCfgBlock() > state.GetCfgBlock().GetPos()) {
                return;
            }
            for (auto& readAnnotation : it.GetAnnotationSources(myReadKind)) {
                if (!readAnnotation.first.GetMemoryOffset().IsSourceObject() || !readAnnotation.first.IsSourceRange()) {
                    // LCOV_EXCL_START
                    continue;
                    // LCOV_EXCL_STOP
                }
                auto tu = varDecl->GetTranslationUnit();
                TracePath problemTrace = {
                    {tu, varDecl->GetRange(), StrLocales::GetStringLocale("UNINITIALIZED_VARIABLE_DECLARATION")},
                    {tu, terminator.GetRange(),
                     StrLocales::GetStringLocale("GOTO_STATEMENT_BEFORE_VARIABLE_DECLARATION")},
                    {tu, readAnnotation.second->GetRange(),
                     StrLocales::GetStringLocale("UNINITIALIZED_VARIABLE_USAGE")}};
                holder.RegisterProblem(*this, *readAnnotation.second,
                                       {{varDecl->GetName()}, problemTrace, "UNINIT.GOTO"});
                break;
            }
            it.ClearAnnotation(myReadKind);
        });
    }

private:
    void FillSinks(DfaState& declState, SuspiciousPath& path, const VarDecl* varDecl, DfaState& state,
                   bool checkPointedDecl = false)
    {
        if (varDecl != nullptr) {
            auto type = varDecl->GetType();
            if (type.IsTrivial() || type.IsPointer()) {
                for (auto& it : declState.GetAnnotationSources(myReadKind)) {
                    if (it.first.GetMemoryOffset().IsSourceObject()) {
                        path.sinkAnnotations.emplace_back(it.first);
                    }
                }
                path.problemKind = "UNINIT.VAR";
                return;
            }
        }

        for (auto& [anno, node] : declState.GetAnnotationSources(myReadKind)) {
            auto readOffset = anno.GetMemoryOffset();
            auto stateOffset = state.GetMemoryOffset();
            auto offset = readOffset;
            auto subOffset = offset.ExtractSubOffset();
            auto typedNode = Node::Cast<TypedNode>(node);

            if (subOffset.first == VirtualOffset::Kind::INDEX && typedNode != nullptr && offset.IsSourceObject() &&
                typedNode->GetDeclaration() != nullptr) {
                auto decl = typedNode->GetDeclaration();
                auto record = Node::Cast<RecordDecl>(decl->GetType().GetDeclaration());
                if (record == nullptr && !checkPointedDecl) {
                    continue;
                }

                record = Node::Cast<RecordDecl>(decl->GetType().GetPointedDeclaration());
                if (record == nullptr) {
                    continue;
                }

                auto& fields = record->GetFields();
                if (subOffset.second >= fields.size() || fields[subOffset.second]->GetInitializer() != nullptr) {
                    continue;
                }

                auto fieldType = fields[subOffset.second]->GetType();

                if (readOffset.IsSubOffsetOf(stateOffset) && readOffset != stateOffset &&
                    (fieldType.IsTrivial() || fieldType.IsPointer())) {
                    path.sinkAnnotations.emplace_back(anno);
                }
            }
        }
        path.problemKind = path.problemKind.empty() ? "UNINIT.FIELD" : path.problemKind;
    }

    bool IsUnsupportedVar(const VarDecl& varDecl)
    {
        auto type = varDecl.GetType();
        return type.IsArray() || varDecl.IsStaticLocal();
    }
};

std::unique_ptr<HCXX::Checker> CreateUninitializedMemoryUsageChecker()
{
    return std::make_unique<UninitializedMemoryUsageChecker>();
}
