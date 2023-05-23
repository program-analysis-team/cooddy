/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.txt.
#ifndef COODDY_ANALYZER_INCLUDE_DFA_PATHMANAGER_H_
#define COODDY_ANALYZER_INCLUDE_DFA_PATHMANAGER_H_

#include <TranslationUnit.h>
#include <ast/CallExpression.h>
#include <ast/FunctionDecl.h>
#include <dfa/Annotation.h>
#include <solver/SolverContext.h>

#include <functional>

namespace HCXX {

class FunctionBehavior;
class ProblemsHolder;
class DfaChecker;
struct SuspiciousPath;
struct Problem;

class DfaFunctionContext {
public:
    // clang-format off
    enum FunctionFlags {
        UNDEFINED         = 0b000001,
        SYSTEM            = 0b000010,
        NO_RETURN         = 0b000100,
        PUBLIC_API        = 0b001000,
        ANNOTATED_TAINTED = 0b010000,
        CLASS_MEMBER      = 0b100000,
        PURE              = 0b1000000
    };
    // clang-format on

    // return true if function is nonstatic class member
    bool IsClassMember() const
    {
        return GetFunctionFlags() & FunctionFlags::CLASS_MEMBER;
    }

    // return true if definition of function wasn't found in project
    bool IsUndefined() const
    {
        return GetFunctionFlags() & FunctionFlags::UNDEFINED;
    }

    /// True if function is declared in headers, which were included as system one
    bool IsSystem() const
    {
        return GetFunctionFlags() & FunctionFlags::SYSTEM;
    }

    /// True if this function is considered "Public API" (not called in translation unit and has ext. linkage).
    bool IsPublicApi() const
    {
        return GetFunctionFlags() & FunctionFlags::PUBLIC_API;
    }

    /// True if this function was annotated with a special macro (see cli arg --taint-macro).
    bool IsAnnotatedTainted() const
    {
        return GetFunctionFlags() & FunctionFlags::ANNOTATED_TAINTED;
    }

    /// True if this function is pure virtual.
    bool IsPure() const
    {
        return GetFunctionFlags() & FunctionFlags::PURE;
    }

    virtual const TranslationUnit& GetTranslationUnit() const = 0;

    virtual const FunctionDecl* GetFunction() const = 0;

    virtual uint32_t GetFunctionFlags() const = 0;

    virtual uint32_t GetInstruction(const Node& node) const = 0;

    virtual FunctionBehavior* GetBehavior() const = 0;

    virtual DfaFunctionContext* GetCalleeContext(Instruction calleeInstr) const = 0;

    virtual const std::vector<DfaFunctionContext*>& GetOverriddenContexts() const = 0;

    virtual std::vector<DfaFunctionContext*> GetCalleesContexts() const = 0;

    virtual const ParamAnnotation& GetAnnotation(uint32_t paramPos) const = 0;

    virtual void AddAnnotation(uint32_t paramPos, const ParamAnnotation& paramAnnotation) const = 0;
    virtual void AddAnnotation(uint32_t paramPos, const Annotation& annotation) const = 0;

    virtual void AddSuspiciousPath(SuspiciousPath&& path) const = 0;

    virtual void CheckSuspiciousPaths(ProblemsHolder& holder) const = 0;

    virtual SourceRange GetSourceRange(Instruction instruction) const = 0;

    virtual Node::Kind GetInstructionKind(Instruction instruction) const = 0;

    // return false if definition of callee function wasn't found in project
    virtual bool IsCalleeDefined(const CallExpression* call) const = 0;

    /// True if some taint problem was found with this function as the source.
    virtual bool IsTaintProblemFound() const = 0;

    /// Initialize SourceId an ReasonCode fields  .
    virtual void SetTaintProblemInfo(Problem& problem) const = 0;

    virtual size_t GetSizeOfAnnotations() const = 0;

    virtual std::string GetFQN() const = 0;

    virtual const std::string& GetSignature() const = 0;

    virtual std::string GetArgName(Instruction argInstr, uint32_t argPos) const = 0;

    virtual std::string GetVarName(Instruction instr) const = 0;

    virtual std::vector<Annotation> CollectAnnotations(const Annotation& source) const = 0;

    virtual UnitLanguage GetLanguage() const = 0;

    // call callback for each leaf annotation that has been propagated from 'annotation'
    using AnnotationPath = std::vector<std::pair<const DfaFunctionContext&, const Annotation&>>;
    using TraverseCallback = const std::function<void(const AnnotationPath&)>&;
    virtual void TraverseAnnotationLocations(const Annotation& annotation, TraverseCallback callback) const = 0;
};

};  // namespace HCXX

#endif  // COODDY_ANALYZER_INCLUDE_DFA_PATHMANAGER_H_
