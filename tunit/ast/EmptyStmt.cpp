/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <TranslationUnit.h>
#include <ast/EmptyStmt.h>

namespace HCXX {

bool EmptyStmt::CheckIfSyntheticStmt() const
{
    const auto* tu = this->GetTranslationUnit();
    std::string text = tu->GetSourceInRange(this->GetRange());
    if (text == ";") {
        return false;
    }

    //  Actually, text is equal "{"
    return true;
}

}  //  namespace HCXX
