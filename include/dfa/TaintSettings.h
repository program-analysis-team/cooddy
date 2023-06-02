/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_INCLUDE_DFA_TAINTSETTINGS_H_
#define COODDY_ANALYZER_INCLUDE_DFA_TAINTSETTINGS_H_

#include <utils/EnumMapping.h>
#include <utils/Log.h>
#include <utils/json-cpp.h>

namespace HCXX {

/// Representation of settings specified by --taint-options on the commandline.
struct TaintSettings {
    // clang-format off
    enum TaintFlags : uint32_t {
        NONE       = 0b0000,
        ANNOTATED  = 0b0001,
        UNDEFINED  = 0b0010,
        PUBLIC_API = 0b0100,
        DEFINED    = 0b1000
    };
    // clang-format on

    uint32_t taintFlags;
    bool taintMacroSummary;

    DECLARE_FIELDS("taint_flags", taintFlags, "taint_macro_summary", taintMacroSummary);

    explicit TaintSettings(uint32_t taintFlags = FlagsDefault(), bool taintMacroSummary = true)
        : taintFlags(taintFlags), taintMacroSummary(taintMacroSummary)
    {}

    explicit TaintSettings(std::string_view taintFlagsCli, bool taintMacroSummary = true)
        : TaintSettings(FlagsFromCli(taintFlagsCli), taintMacroSummary)
    {}

    static constexpr std::string_view DEFAULT_FLAGS = "sup";

    static uint32_t FlagsFromCli(std::string_view cliArg);
    static uint32_t FlagsDefault();
};

/**
 * @brief Parse --taint-options CLI argument into an instance of TaintSettings
 * @return TaintSettings parsed from cliArg or default TaintSettings if cliArg was incorrect.
 */
inline uint32_t TaintSettings::FlagsFromCli(std::string_view cliArg)
{
    auto wrongCli = [&]() {
        Log(HCXX::LogLevel::ERROR) << "Invalid value \"" << cliArg << "\" for command-line parameter taint-options"
                                   << std::endl;
        return FlagsDefault();
    };

    if (cliArg == "none") {
        return TaintFlags::NONE;
    }

    if (cliArg == "all") {
        return TaintFlags::ANNOTATED | TaintFlags::UNDEFINED | TaintFlags::PUBLIC_API | TaintFlags::DEFINED;
    }

    if (cliArg.empty()) {
        return wrongCli();
    }

    uint32_t result = TaintFlags::NONE;
    for (auto ch : cliArg) {
        switch (ch) {
            case 'u':
                result |= TaintFlags::UNDEFINED;
                break;
            case 's':
                result |= TaintFlags::ANNOTATED;
                break;
            case 'p':
                result |= TaintFlags::PUBLIC_API;
                break;
            default:
                return wrongCli();
        }
    }
    return result;
}

/// Default TaintSettings, based on default value for --taint-options CLI argument.
inline uint32_t TaintSettings::FlagsDefault()
{
    static uint32_t result = FlagsFromCli(DEFAULT_FLAGS);
    return result;
}

}  // namespace HCXX

#endif  // COODDY_ANALYZER_INCLUDE_DFA_TAINTSETTINGS_H_
