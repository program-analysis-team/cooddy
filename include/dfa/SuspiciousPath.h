/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_INCLUDE_DFA_SUSPICIOUSPATH_H_
#define COODDY_ANALYZER_INCLUDE_DFA_SUSPICIOUSPATH_H_

namespace HCXX {

struct SuspiciousPath {
    SuspiciousPath(DfaChecker& checker, Annotation::Kind sinkKind, Annotation::Kind sourceKind,
                   const std::string& sourceName = "", const Condition& sinkCondition = {}, uint32_t checkPathFlags = 0)
        : checker(&checker),
          sourceName(sourceName),
          sinkKind(sinkKind),
          sourceKind(sourceKind),
          sinkCondition(sinkCondition),
          checkPathFlags(checkPathFlags)
    {}
    ~SuspiciousPath() = default;

    DfaChecker* checker = nullptr;
    std::string problemKind;
    std::string sourceName;
    Annotation::Kind sinkKind;
    Annotation::Kind sourceKind;
    Condition sinkCondition;
    std::vector<Annotation> sinkAnnotations;
    std::vector<Annotation> sourceAnnotations;
    uint32_t checkPathFlags = 0;
    uint32_t checkPriority = 0;
    std::optional<VirtualOffset> sourceOffset = std::nullopt;
    Instruction jointInstruction = 0;
};

}  // namespace HCXX

#endif  // COODDY_ANALYZER_INCLUDE_DFA_SUSPICIOUSPATH_H_
