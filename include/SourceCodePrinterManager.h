/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_SOURCECODEPRINTERMANAGER_H
#define COODDY_SOURCECODEPRINTERMANAGER_H

#include <atomic>
#include <string>
class SourceCodePrinterManager {
public:
    std::string GetNextName(bool simplify = true)
    {
        if (simplify) {
            return {static_cast<char>(myVarNum++ % 26 + 97)};
        }
        return "v_" + std::to_string(myVarNum++);  // LCOV_EXCL_LINE
    }

private:
    std::atomic<int> myVarNum = 0;
};

#endif  // COODDY_SOURCECODEPRINTERMANAGER_H
