/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef SUPPORTED_LANGUGES_HEADER_FILE
#define SUPPORTED_LANGUGES_HEADER_FILE

#include <optional>
#include <string>
#include <vector>

namespace HCXX {

class SupportedLanguages {
public:
    SupportedLanguages() = default;
    ~SupportedLanguages() = default;

    bool IsCppLanguageSupported() const;
    bool IsPureCLanguageSupported() const;

public:
    //  it returns nullopt if the parameter isn't valid
    static std::optional<SupportedLanguages> FromArr(const std::vector<std::string>&);

private:
    explicit SupportedLanguages(int);

private:
    enum LanguagesMask { LANG_C = 0x01, LANG_CPP = 0x02 };

    uint32_t myMask = LANG_C | LANG_CPP;
};

}  //  namespace HCXX

#endif  // #ifndef SUPPORTED_LANGUGES_HEADER_FILE
