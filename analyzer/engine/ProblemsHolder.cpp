/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <Checker.h>
#include <ProblemsHolder.h>
#include <ast/CxxRecordDecl.h>
#include <deps/md5.h>
#include <workspace/Workspace.h>

using namespace HCXX;
static const uint32_t MAX_SNIPPET_SIZE = 300;

static void FillScopeRangeAndName(const Node& node, Problem& problem)
{
    auto funcDecl = node.GetParentOfType<FunctionDecl>();
    if (funcDecl == nullptr) {
        problem.scopeRange = SourceRange();
        return;
    }
    auto templateFunction = funcDecl->GetTemplateInstantiationPattern();
    if (templateFunction != nullptr && templateFunction->GetRange().IsValid()) {
        problem.scopeRange = templateFunction->GetRange();
        problem.functionName = templateFunction->GetQualifiedName();
        return;
    }
    problem.scopeRange = funcDecl->GetRange();
    problem.functionName = funcDecl->GetQualifiedName();
}

static std::string GetStrippedSnipped(const std::string& source, const SourceRange& range, int32_t scopeBegin)
{
    std::string result;
    int32_t rangeEnd = std::min(range.end + 1, uint32_t(source.size()));
    if (rangeEnd <= 0) {
        // LCOV_EXCL_START
        return result;
        // LCOV_EXCL_STOP
    }
    int32_t rangeBegin = std::min(std::max(int32_t(range.begin), 0), rangeEnd - 1);
    int32_t lastLine = 0;
    bool isComment = false;

    for (int32_t p = rangeBegin - 1; p >= 0 && p >= scopeBegin && result.size() < MAX_SNIPPET_SIZE; --p) {
        if (isComment) {
            if (p > 0 && source[p - 1] == '/' && source[p] == '*') {
                isComment = false;
                p--;
            }
        } else if (source[p] == '\n') {
            lastLine = result.size();
        } else if (p > 0 && source[p - 1] == '*' && source[p] == '/') {
            isComment = true;
            p--;
        } else if (p > 0 && source[p - 1] == '/' && source[p] == '/') {
            result.erase(result.begin() + lastLine, result.end());
            p--;
        } else if (source[p] != '\r' && source[p] != '\t' && source[p] != ' ' || source[p] == source[rangeBegin]) {
            result += source[p];
        }
    }
    result.append(&source[rangeBegin], rangeEnd - rangeBegin);
    return result;
}

ProblemsHolder::ProblemsHolder(const Workspace& workspace) : myWorkspace(workspace), myProblemsStream(true)
{
    myNewProblemsMode = workspace.GetOptions().incrementalSettings.reportNewProblems;
}

UniqueId ProblemsHolder::CalcProblemUniqueId(const Problem& problem, const TranslationUnit& unit,
                                             const SourceRange& range)
{
    UniqueId result;
    MD5_CTX md5ctx;
    MD5_Init(&md5ctx);
    MD5_Update(&md5ctx, problem.checkerName.data(), problem.checkerName.size());

    if (!problem.sourceId.empty() && problem.isSourceIdUnique) {
        MD5_Update(&md5ctx, problem.sourceId.data(), problem.sourceId.size());
        MD5_Final(result.Data(), &md5ctx);
        return result;
    }
    const FileEntry* entry = nullptr;
    Location entryOffset = 0;
    auto it = unit.GetFileEntries().upper_bound(range.begin);
    if (it != unit.GetFileEntries().begin()) {
        entry = &(--it)->second;
        entryOffset = it->first;
    }
    SourceRange entryRange = {range.begin - entryOffset, range.end - entryOffset};
    if (entry == nullptr || entry->isLoaded) {
        auto& entryName = entry == nullptr ? unit.GetMainFileName() : entry->filePath;
        auto fileName = std::filesystem::path(entryName).filename().string();
        MD5_Update(&md5ctx, fileName.data(), fileName.size());
        MD5_Update(&md5ctx, &entryRange, sizeof(entryRange));
        MD5_Final(result.Data(), &md5ctx);
        return result;
    }

    int32_t scopeBegin = std::max(0, int32_t(problem.scopeRange.begin - it->first));
    auto strippedSnippet = GetStrippedSnipped(entry->fileSource, entryRange, scopeBegin);
    MD5_Update(&md5ctx, strippedSnippet.data(), strippedSnippet.size());

    auto p = entry->fileSource.find_first_of(";{}", scopeBegin);
    if (p != std::string::npos && problem.scopeRange.IsValid()) {
        strippedSnippet = GetStrippedSnipped(entry->fileSource, {uint32_t(p), uint32_t(p)}, scopeBegin);
        MD5_Update(&md5ctx, strippedSnippet.data(), strippedSnippet.size());
    }

    auto filePath = myWorkspace.GetRelativePath(entry->filePath);
    MD5_Update(&md5ctx, filePath.data(), filePath.size());
    MD5_Final(result.Data(), &md5ctx);
    return result;
}

void ProblemsHolder::RegisterProblem(Checker& checker, const Node& node, Problem&& problem)
{
    if (!problem.scopeRange.IsValid()) {
        FillScopeRangeAndName(node, problem);
    }
    RegisterProblem(checker, *node.GetTranslationUnit(), node.GetRange(), std::move(problem));
}

void ProblemsHolder::RegisterProblem(Checker& checker, const TranslationUnit& unit, const SourceRange& range,
                                     Problem&& problem)
{
    if (problem.checkerName.empty()) {
        problem.checkerName = checker.GetName();
    }

    auto& inspection = myWorkspace.GetProfile().GetInspectionCfg(problem.checkerName, problem.kind);
    if (problem.kind.empty()) {
        problem.kind = inspection.id;
    }

    if (problem.profileName.empty()) {
        problem.profileName = inspection.profileName;
    }

    if (problem.description.empty()) {
        problem.description = StrUtils::ReplacePlaceholders(inspection.description, problem.replacements);
    }
    if (problem.trace.empty() && !inspection.noTrace) {
        problem.trace = {TraceNode(&unit, range, problem.description)};
    }
    if (problem.severity == Problem::Severity::UNDEF) {
        problem.severity = inspection.severity;
    }
    if (!problem.scopeRange.IsValid() && checker.GetKind() == Checker::Kind::AST_CHECKER) {
        auto node = unit.FindContainingNode(range.begin);
        if (node != nullptr) {
            FillScopeRangeAndName(*node, problem);
        }
    }

    problem.uniqueId = CalcProblemUniqueId(problem, unit, range);

    if (RegisterProblem(std::move(problem), unit, range, checker.GetKind() != Checker::Kind::GLOBAL_CHECKER)) {
        ++checker.problemsFound;
    }
}

bool ProblemsHolder::RegisterProblem(Problem&& problem, const TranslationUnit& unit, const SourceRange& range,
                                     bool store)
{
    LocationInfo loc = unit.GetLocationInfo(range);
    if (loc.filename.empty() ||
        (!myWorkspace.GetOptions().ignoreSuppress && unit.CheckSuppress(problem.checkerName, loc))) {
        return false;
    }
    static_cast<LocationInfoBase&>(problem) = loc;
    problem.filename = loc.filename;

    if (myWorkspace.GetExclusionManager().IsProblemExcluded(problem)) {
        return false;
    }

    for (auto& trace : problem.trace) {
        if (!trace.scopeRange.IsValid() && trace.tu == &unit) {
            trace.scopeRange = problem.scopeRange;
        }
        if (trace.tu != nullptr && !trace.IsInitialized()) {
            auto traceLoc = trace.tu->GetLocationInfo(trace.range);
            static_cast<LocationInfoBase&>(trace) = traceLoc;
            trace.filename = traceLoc.filename;
        }
        if (trace.line == 0 || !trace.IsInitialized()) {
            // LCOV_EXCL_START
            Log(LogLevel::ERROR) << "Problem trace is invalid:\n"
                                 << problem.checkerName << " " << problem.filename << ":" << problem.line << ":"
                                 << problem.column << std::endl;
            return false;
            // LCOV_EXCL_STOP
        }
    }

    std::unique_lock<std::mutex> lock(myMutex);
    if (!myProblems.insert(problem.uniqueId).second) {
        return false;
    }
    if (store) {
        AddStoredProblem(unit, range, problem);
    }
    if (!myNewProblemsMode) {
        RegisterProblemImpl(problem);
    }
    return true;
}

void ProblemsHolder::AddStoredProblem(const TranslationUnit& unit, const SourceRange& range, Problem& problem)
{
    if (!myNewProblemsMode && !myWorkspace.GetOptions().incrementalSettings.writeMode) {
        return;
    }
    auto entry = FileEntriesCache::GetInstance().GetFileEntry(unit.GetMainFileName());
    myProblemsStream.Add<UniqueId>(entry != nullptr ? entry->hash : UniqueId());
    myProblemsStream.Add<SourceRange>(range);
    problem.Serialize(myProblemsStream);

    myProblemsStream.Add<uint16_t>(problem.trace.size());
    for (auto& it : problem.trace) {
        entry = FileEntriesCache::GetInstance().GetFileEntry(it.tu->GetMainFileName());
        myProblemsStream.Add<UniqueId>(entry != nullptr ? entry->hash : UniqueId());
        it.Serialize(myProblemsStream);
    }
}

void ProblemsHolder::ReportStoredProblem(IOStream& problemsStream, UnitsMapping& unitsMapping, bool newProblem)
{
    auto unit = unitsMapping[problemsStream.Get<UniqueId>()];
    auto range = problemsStream.Get<SourceRange>();

    Problem problem;
    problem.Serialize(problemsStream);

    uint32_t traceSize = problemsStream.Get<uint16_t>();
    for (uint32_t i = 0; i < traceSize; i++) {
        TraceNode traceNode;
        traceNode.tu = unitsMapping[problemsStream.Get<UniqueId>()];
        traceNode.Serialize(problemsStream);
        if (traceNode.tu != nullptr) {
            problem.trace.emplace_back(std::move(traceNode));
        }
    }
    if (unit == nullptr || !newProblem && unit->IsInited()) {
        // if this is baseline problem in changed TU
        if (myNewProblemsMode) {
            myProblems.insert(problem.uniqueId);
        }
    } else if (!RegisterProblem(std::move(problem), *unit, range, true)) {
        if (newProblem && !myProblems.insert(problem.uniqueId).second) {
            // if new problem is already added to map from baseline, then store it for update
            AddStoredProblem(*unit, range, problem);
        }
    }
}

void ProblemsHolder::FlushProblems(const std::string& streamName, UnitsMapping& unitsMapping)
{
    IOStream newProblemsStream(false);
    if (myNewProblemsMode) {
        newProblemsStream.AddBuffer(myProblemsStream.GetData(), myProblemsStream.Count());
        myProblemsStream = IOStream(true);
        myProblems.clear();
    }
    if (myWorkspace.GetOptions().incrementalSettings.readMode) {
        IOStream problemsStream(false);
        myWorkspace.GetAnalysisData(streamName, problemsStream);
        while (!problemsStream.IsEOF()) {
            ReportStoredProblem(problemsStream, unitsMapping, false);
        }
    }
    myNewProblemsMode = false;
    while (!newProblemsStream.IsEOF()) {
        ReportStoredProblem(newProblemsStream, unitsMapping, true);
    }
    if (myWorkspace.GetOptions().incrementalSettings.writeMode) {
        myWorkspace.StoreAnalysisData(streamName, myProblemsStream);
    }
}
