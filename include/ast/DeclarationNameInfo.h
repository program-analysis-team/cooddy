/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_DECLARATIONNAMEINFO_H
#define COODDY_DECLARATIONNAMEINFO_H

#include <ast/Node.h>

#include <string>

namespace HCXX {

class DeclarationNameInfo {
public:
    DeclarationNameInfo() {}
    DeclarationNameInfo(const std::string& name, const SourceRange& sourceRange)
        : myName(name), mySourceRange(sourceRange)
    {}

    const std::string& GetName() const
    {
        return myName;
    }

    const SourceRange& GetRange() const
    {
        return mySourceRange;
    }

private:
    std::string myName;
    SourceRange mySourceRange;
};

}  // namespace HCXX

#endif  // COODDY_DECLARATIONNAMEINFO_H
