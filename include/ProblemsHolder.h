/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
//
// ProblemsHolder class declares base interface for problems registration in checkers implementation.
// This class doesn't store found problems, it's responsibility of derived classes.
//

#ifndef HCXXANALYZER_PROBLEMSHOLDER_H
#define HCXXANALYZER_PROBLEMSHOLDER_H

#include <TranslationUnit.h>
#include <dfa/Annotation.h>
#include <utils/DataStream.h>
#include <utils/EnumMapping.h>
#include <utils/UniqueId.h>

#include <string>
#include <string_view>
#include <vector>

namespace HCXX {

class Checker;
class Workspace;
class DfaFunctionContext;

using Replacements = std::vector<std::string>;

struct TraceNode : public LocationInfoBase {
    DECLARE_ENUM(Kind, DEFAULT, ASSUMPTION, BRANCH, WEAKASSUMPTION);
    TraceNode(const TranslationUnit* tu, const SourceRange& range, const std::string_view desc,
              const Annotation& annotation = Annotation(), TraceNode::Kind kind = Kind::DEFAULT,
              SourceRange&& scopeRange = {})
        : tu(tu), range(range), description(desc), kind(kind), annotation(annotation), scopeRange(scopeRange){};
    TraceNode(){};
    ~TraceNode() = default;

    void Serialize(IOStream& stream)
    {
        stream << range << description << kind << scopeRange;
        stream.SerializePod(annotation);
    }

    const TranslationUnit* tu = nullptr;
    SourceRange range;
    std::string description;
    Kind kind = Kind::DEFAULT;
    Annotation annotation;
    SourceRange scopeRange;  // range of the location's scope
    std::string filename;
    int parentId = -1;
    int id = 0;
    const DfaFunctionContext* funcCtx = nullptr;
};
using TracePath = std::vector<TraceNode>;

struct Problem : public LocationInfoBase {
    DECLARE_ENUM(Severity, ERROR, WARNING, NOTIFY, UNDEF);
    Problem(){};
    Problem(const Replacements& replacements, const TracePath& trace = {}, std::string_view kind = {},
            std::string_view checkerName = {})
        : replacements(replacements), trace(trace), kind(kind), checkerName(checkerName)
    {}

    void Serialize(IOStream& stream)
    {
        stream << severity << codeLanguage << kind << profileName << checkerName << description << sourceId
               << reasonCode << functionName << sinkFunction << scopeRange;
        stream.SerializePod(uniqueId);
    }

    Replacements replacements;  // problem custom parameters to substitute in description
    TracePath trace;            // problem trace from DfaChecker
    Severity severity = Severity::UNDEF;
    std::string codeLanguage = "c/c++";  // Source code language for reported problem

    std::string filename;
    std::string kind;
    std::string profileName;
    std::string checkerName;
    std::string description;
    std::string sourceId;         // some id which identifies problem's source
    std::string reasonCode;       // reason why problem is reported
    std::string functionName;     // name of function where problem is located
    std::string sinkFunction;     // name of function where problem's sink is annotated
    SourceRange scopeRange;       // range of the problem's scope (function range)
    uint32_t solverDuration = 0;  // duration required to solve z3 equations (in milliseconds)
    bool isSourceIdUnique = false;
    UniqueId uniqueId;
    std::string traceReport;  // path to the file with problem's trace

    /// Debug print operator
    friend inline std::ostream& operator<<(std::ostream& os, const Problem& obj)
    {
        os << "<" << obj.checkerName << ", " << obj.filename << ":" << obj.line << ":" << obj.column << ">";

        return os;
    }

    /// Converts severity from case-insensitive string
    static Severity SeverityFromCaseInsensitiveString(const string& op)
    {
        return Problem::SeverityFromStr(HCXX::StrUtils::BigSnakeCaseToCamelCase(StrUtils::ToUpper(op)));
    }
};

class ProblemsHolder {
public:
    // LCOV_EXCL_START
    explicit ProblemsHolder(const Workspace& workspace);
    virtual ~ProblemsHolder() = default;

    virtual void RegisterProblem(Checker& checker, const Node& node, Problem&& problem = {});

    virtual void RegisterProblem(Checker& checker, const TranslationUnit& unit, const SourceRange& range,
                                 Problem&& problem = {});

    using UnitsMapping = std::unordered_map<UniqueId, const TranslationUnit*, UniqueId::HashFunction>;
    virtual void FlushProblems(const std::string& streamName, UnitsMapping& unitsMapping);

    virtual void Flush(){};

    const Workspace& GetWorkspace() const
    {
        return myWorkspace;
    }

protected:
    virtual void RegisterProblemImpl(const Problem& problem) {}
    // LCOV_EXCL_STOP

    const Workspace& myWorkspace;

private:
    void AddStoredProblem(const TranslationUnit& unit, const SourceRange& range, Problem& problem);

    bool RegisterProblem(Problem&& problem, const TranslationUnit& unit, const SourceRange& range, bool store);

    void ReportStoredProblem(IOStream& problemsStream, UnitsMapping& unitsMapping, bool newProblem);

    UniqueId CalcProblemUniqueId(const Problem& problem, const TranslationUnit& unit, const SourceRange& range);

    std::unordered_set<UniqueId, UniqueId::HashFunction> myProblems;
    IOStream myProblemsStream;
    std::mutex myMutex;
    bool myNewProblemsMode = false;
};
}  // namespace HCXX

#endif  // HCXXANALYZER_PROBLEMSHOLDER_H
