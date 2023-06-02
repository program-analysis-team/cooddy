/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_SOURCE_DFA_FUNCTIONCONTEXT_H_
#define COODDY_ANALYZER_SOURCE_DFA_FUNCTIONCONTEXT_H_

#include <Analyzer.h>
#include <cfg/Cfg.h>
#include <dfa/DfaChecker.h>
#include <dfa/DfaFunctionContext.h>
#include <solver/FunctionBehavior.h>
#include <utils/ThreadPool.h>

#include <memory>
#include <unordered_map>

#include "RecordsTree.h"

using namespace HCXX;

class DataFlowAnalyzer;

class FunctionContext : public DfaFunctionContext {
public:
    FunctionContext(DataFlowAnalyzer& analyzer, const RecordsTree& recordsTree, const std::string& signature);
    ~FunctionContext();

    void SetFunctionFlags(uint32_t functionFlags, const FunctionDecl* function, UnitLanguage lang);

    uint32_t GetFunctionFlags() const override
    {
        return myFunctionFlags;
    }

    const FunctionDecl* GetFunction() const override
    {
        return myFunction;
    }

    const TranslationUnit& GetTranslationUnit() const override
    {
        return *myUnit;
    }

    FunctionBehavior* GetBehavior() const override
    {
        return myBehavior.get();
    }

    const std::string& GetSignature() const override
    {
        return mySignatureRef;
    }

    std::string GetFQN() const override;

    const auto& GetCallees() const
    {
        return myCallees;
    }

    std::vector<DfaFunctionContext*> GetCalleesContexts() const override;

    void AddOverriddenContext(RecordUniqueId recordId, FunctionContext& funcCtx);

    using CalleeRef = std::pair<uint32_t, FunctionContext*>;
    CalleeRef GetOverriddenContext(RecordUniqueId recordId);

    const std::vector<DfaFunctionContext*>& GetOverriddenContexts() const override
    {
        return myOverriddenContexts;
    }

    void SortOverriddenContexts();

    void SetInited()
    {
        myIsInited = true;
    }

    void CleanUp();

    bool SetAnalyzed();

    bool IsAnalyzed() const
    {
        return myIsAnalyzed || IsUndefined();
    }

    bool IsAttached() const
    {
        return myIsAttached;
    }

    bool HasConditions() const
    {
        return myHasConditions;
    }

    bool IsTaintProblemFound() const override
    {
        return myIsTaintProblemFound;
    }

    void Attach(TranslationUnitPtr& unit, std::shared_ptr<Cfg> cfg);

    void AddCallee(Instruction calleeInstr, FunctionContext& calleeCtx, uint32_t overriddenIndex = 0);

    FunctionContext* GetCallee(Instruction instruction) const;

    SourceRange GetSourceRange(Instruction instruction) const override
    {
        return myBehavior->GetSourceRange(instruction);
    }

    Node::Kind GetInstructionKind(Instruction instruction) const override;

    void GetAnalysisOrder(uint32_t& revisionId, std::vector<FunctionContext*>& order);
    void SortAnalysisOrder(uint32_t& revisionId, std::vector<FunctionContext*>& order);
    void SortLinkedComponent(uint32_t compId, std::vector<FunctionContext*>& order);
    void MakeLinkedComponent(uint32_t revisionId, uint32_t compId, FunctionContext*& compRoot);

    void Analyze(ProblemsHolder& holder, PoolTask& poolTask, Analyzer::Consumer& consumer);

    uint32_t GetInstruction(const Node& node) const override
    {
        return myBehavior->GetInstruction(node);
    }

    DfaFunctionContext* GetCalleeContext(Instruction instruction) const override
    {
        return GetCallee(instruction);
    }

    /**
     * @brief A vector of annotations for (almost) each parameter and the return value.
     *
     * The returned vector<vector<Annotation>> is not always the same size as the number of params + retvalue.
     * If some parameters at the end are missing annotations, the outer vector will be shorter to save space on empty
     * vectors. Following this logic, the vector can be empty if neither the params nor the retvalue have any
     * annotations.
     *
     * Despite this, when a param in the middle does not have annotations but params after it do, the vector will extend
     * to the last param with annotations (i.e. vector only skips params at the very end).
     */
    const FunctionAnnotation& GetAnnotation() const
    {
        return myAnnotation;
    }

    size_t GetSizeOfAnnotations() const override
    {
        return myAnnotation.size();
    }

    const ParamAnnotation& GetAnnotation(uint32_t paramPos) const override;

    void AddAnnotation(uint32_t paramPos, const ParamAnnotation& annotation) const override;
    void AddAnnotation(uint32_t paramPos, const Annotation& annotation) const override;

    void AddSuspiciousPath(SuspiciousPath&& path) const override;

    void CheckSuspiciousPaths(ProblemsHolder& holder) const override;

    void TraverseAnnotationLocations(const Annotation& annotation, TraverseCallback callback) const override;

    bool LoadAnalysisData(TranslationUnitPtr& unit, DataStream& stream, std::vector<Annotation::Kind>& mapping);

    void StoreAnalysisData(DataStream& stream);

    bool IsCalleeDefined(const CallExpression* call) const override;

    bool AddToAnalysisOrder(uint32_t revisionId, std::vector<FunctionContext*>& order);

    std::vector<Annotation> CollectAnnotations(const Annotation& source) const override;

    void TraverseAnnotationLocationsHelper(const Annotation& annotation, TraverseCallback callback,
                                           AnnotationPath& path) const;

    std::string GetTaintProblemReasonCode() const;
    void SetTaintProblemInfo(Problem& problem) const override;

    std::string GetArgName(Instruction argInstr, uint32_t argPos) const override;

    std::string GetVarName(Instruction instr) const override;

    void SortAnnotations();

    UnitLanguage GetLanguage() const
    {
        return myLanguage;
    }

private:
    static constexpr uint32_t OVERRIDDEN_BIT = (1UL << 31);
    static constexpr uint32_t MAX_OVERRIDDEN_CONTEXTS = 16;

    DataFlowAnalyzer& myAnalyzer;
    const RecordsTree& myRecordsTree;
    const std::string& mySignatureRef;
    TranslationUnitPtr myUnit;
    std::shared_ptr<Cfg> myCfg;
    NodePtr<FunctionDecl> myFunction = nullptr;
    mutable FunctionAnnotation myAnnotation;
    mutable std::vector<SuspiciousPath> mySuspiciousPaths;
    std::vector<DfaFunctionContext*> myOverriddenContexts;
    std::vector<FunctionContext*> myCallers;
    std::unordered_map<Instruction, CalleeRef> myCallees;
    std::unique_ptr<FunctionBehavior> myBehavior;
    std::atomic<uint32_t> myAnalysingCounter = 0;
    std::atomic<uint32_t> myAnalyzedCounter = 0;
    uint32_t myMaxAnalysingCounter = 0;
    uint32_t myRevisionId = 0;
    uint32_t myVariadicPos = 0;
    uint32_t myFunctionFlags = 0;
    UnitLanguage myLanguage = UnitLanguage::C;
    mutable bool myIsTaintProblemFound = false;
    bool myRevisionResult = true;
    bool myIsAnalyzed = false;
    bool myIsInited = false;
    bool myIsAttached = false;
    bool myHasConditions = false;
};

#endif  // COODDY_ANALYZER_SOURCE_DFA_FUNCTIONCONTEXT_H_
