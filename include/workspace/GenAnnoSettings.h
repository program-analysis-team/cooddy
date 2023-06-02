/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_INCLUDE_WORKSPACE_ANNO_GEN_H_
#define COODDY_ANALYZER_INCLUDE_WORKSPACE_ANNO_GEN_H_

#include <utils/EnumMapping.h>
#include <utils/Log.h>
#include <utils/json-cpp.h>

namespace HCXX {

struct GenAnnoSettings {
    enum GenAnnoFlags : uint32_t { NONE = 0b0000, ANNOTATED = 0b0001, UNDEFINED = 0b0010, SYSTEM = 0b0100 };

    bool IsDisabled() const
    {
        return genAnnoFlags == GenAnnoFlags::NONE;
    }

    uint32_t genAnnoFlags = GenAnnoFlags::NONE;
    DECLARE_FIELDS("gen_anno_Flags", genAnnoFlags);

    static constexpr std::string_view DEFAULT_CLI_ARG = "none";
    static GenAnnoSettings FromCliArg(std::string_view cliArg);
    static GenAnnoSettings Default();
};

inline GenAnnoSettings GenAnnoSettings::FromCliArg(std::string_view cliArg)
{
    GenAnnoSettings result;

    auto wrongCli = [&]() {
        Log(HCXX::LogLevel::ERROR) << "Invalid value \"" << cliArg << "\" for command-line parameter gen-annotations"
                                   << std::endl;
        return Default();
    };

    if (cliArg == "none") {
        result.genAnnoFlags = GenAnnoFlags::NONE;
        return result;
    }

    if (cliArg == "all") {
        result.genAnnoFlags = GenAnnoFlags::ANNOTATED | GenAnnoFlags::UNDEFINED | GenAnnoFlags::SYSTEM;
        return result;
    }

    if (cliArg.empty()) {
        return wrongCli();
    }

    for (auto ch : cliArg) {
        switch (ch) {
            case 'u':
                result.genAnnoFlags |= GenAnnoFlags::UNDEFINED;
                break;
            case 't':
                result.genAnnoFlags |= GenAnnoFlags::ANNOTATED;
                break;
            case 's':
                result.genAnnoFlags |= GenAnnoFlags::SYSTEM;
                break;
            default:
                return wrongCli();
        }
    }
    return result;
}

inline GenAnnoSettings GenAnnoSettings::Default()
{
    static GenAnnoSettings result = FromCliArg(DEFAULT_CLI_ARG);
    return result;
}

}  // namespace HCXX

#endif  // COODDY_ANALYZER_INCLUDE_WORKSPACE_ANNO_GEN_H_
