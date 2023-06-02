/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <set>

#include "ParserImpl.h"

using namespace HCXX;

const std::unordered_set<std::string> g_EXCLUDED_OPTIONS = {"--compatible-with-void-pointers",
                                                            "--coverage",
                                                            "--trace-includes",
                                                            "-H",
                                                            "-MD",
                                                            "-MF",
                                                            "-MQ",
                                                            "-MT",
                                                            "-c",
                                                            "-coverage",
                                                            "-fPIC",
                                                            "-fPIE",
                                                            "-faddrsig",
                                                            "-fasynchronous-unwind-tables",
                                                            "-fbranch-target-load-optimize",
                                                            "-fconserve-stack",
                                                            "-fcx-fortran-rules",
                                                            "-ffixed-r2",
                                                            "-ffloat-store",
                                                            "-fgcse-after-reload",
                                                            "-fgcse-las",
                                                            "-finline-functions-called-once",
                                                            "-finsert-func-section",
                                                            "-fipa-cp-clone",
                                                            "-fira-loop-pressure",
                                                            "-fno-PIE",
                                                            "-fno-addrsig",
                                                            "-fno-aggressive-loop-optimizations",
                                                            "-fno-asynchronous-unwind-tables",
                                                            "-fno-branch-count-reg",
                                                            "-fno-code-hoisting",
                                                            "-fno-dwarf2-cfi-asm",
                                                            "-fno-exceptions",
                                                            "-fno-forward-propagate",
                                                            "-fno-gcse",
                                                            "-fno-guess-branch-probability",
                                                            "-fno-if-conversion",
                                                            "-fno-ipa-cp",
                                                            "-fno-ipa-icf",
                                                            "-fno-ipa-icf-functions",
                                                            "-fno-ipa-ra",
                                                            "-fno-ipa-sra",
                                                            "-fno-merge-all-constants",
                                                            "-fno-merge-constants",
                                                            "-fno-omit-frame-pointer",
                                                            "-fno-optimize-sibling-calls",
                                                            "-fno-partial-inlining",
                                                            "-fno-pic",
                                                            "-fno-sched-interblock",
                                                            "-fno-sched-spec-insn-heuristic",
                                                            "-fno-schedule-insns",
                                                            "-fno-schedule-insns2",
                                                            "-fno-semantic-interposition",
                                                            "-fno-short-enums",
                                                            "-fno-short-wchar",
                                                            "-fno-stack-check",
                                                            "-fno-stack-protector",
                                                            "-fno-strict-aliasing",
                                                            "-fno-strict-overflow",
                                                            "-fno-tree-dominator-opts",
                                                            "-fno-tree-sink",
                                                            "-fno-tree-slp-vectorize",
                                                            "-fno-tree-vectorize",
                                                            "-fno-tree-vrp",
                                                            "-fno-unwind-tables",
                                                            "-fno-var-tracking",
                                                            "-fno-web",
                                                            "-fomit-frame-pointer",
                                                            "-foptimize-sibling-calls",
                                                            "-fpic",
                                                            "-fpredictive-commoning",
                                                            "-frecord-gcc-switches",
                                                            "-freschedule-modulo-scheduled-loops",
                                                            "-frtti",
                                                            "-fsched2-use-superblocks",
                                                            "-fsel-sched-reschedule-pipelined",
                                                            "-fshort-wchar",
                                                            "-fsigned-char",
                                                            "-fstack-protector",
                                                            "-fstack-protector-all",
                                                            "-fstack-protector-strong",
                                                            "-fstrict-aliasing",
                                                            "-ftree-loop-distribute-patterns",
                                                            "-ftree-loop-distribution",
                                                            "-ftree-vectorize",
                                                            "-funsigned-char",
                                                            "-funswitch-loops",
                                                            "-funwind-tables",
                                                            "-fvariable-expansion-in-unroller",
                                                            "-fverbose-asm",
                                                            "-m16",
                                                            "-m32",
                                                            "-m64",
                                                            "-mx32",
                                                            "-marm",
                                                            "-mbig-endian",
                                                            "-mbss-plt",
                                                            "-mgeneral-regs-only",
                                                            "-mhard-float",
                                                            "-mindirect-branch-register",
                                                            "-mlittle-endian",
                                                            "-mlong-calls",
                                                            "-mmmx",
                                                            "-mmultiple",
                                                            "-mno-3dnow",
                                                            "-mno-80387",
                                                            "-mno-altivec",
                                                            "-mno-avx",
                                                            "-mno-fp-ret-in-387",
                                                            "-mno-mmx",
                                                            "-mno-red-zone",
                                                            "-mno-sched-prolog",
                                                            "-mno-spe",
                                                            "-mno-sse",
                                                            "-mno-sse2",
                                                            "-mno-string",
                                                            "-mno-unaligned-access",
                                                            "-mno-vsx",
                                                            "-mskip-rax-setup",
                                                            "-no-canonical-prefixes",
                                                            "-nostartfiles",
                                                            "-nostdinc",
                                                            "-nostdlib",
                                                            "-nostdlibinc",
                                                            "-pie",
                                                            "-pipe",
                                                            "-r",
                                                            "-rdynamic",
                                                            "-s",
                                                            "-v"};

const std::set<std::string> g_EXCLUDED_OPTIONS_STARTS_WITH = {"--param",
                                                              "--specs",
                                                              "--target",
                                                              "-B",
                                                              "-G",
                                                              "-MF",
                                                              "-O",
                                                              "-W",
                                                              "-falign",
                                                              "-finline-limit",
                                                              "-fmax-errors",
                                                              "-fplugin",
                                                              "-fsanitize",
                                                              "-fvisibility",
                                                              "-g",
                                                              "-l",
                                                              "-mabi",
                                                              "-march",
                                                              "-mcmodel",
                                                              "-mcpu",
                                                              "-mfloat-abi",
                                                              "-mfpu",
                                                              "-mindirect-branch",
                                                              "-mpreferred-stack-boundary",
                                                              "-mregparm",
                                                              "-mspe",
                                                              "-msse",
                                                              "-mtune",
                                                              "-z"};

const std::unordered_set<std::string> g_EXCLUDED_OPTIONS_WITH_PARAM = {"-o", "-z", "--param", "--target", "-Xclang"};

void ParserImpl::ConvertOptions(CompilerOptions& options)
{
    for (auto i = options.options.size() - 1; i > 0; --i) {
        auto& op = options.options[i];
        if (g_EXCLUDED_OPTIONS_WITH_PARAM.count(op)) {
            auto count = i != options.options.size() - 1 ? 2 : 1;
            options.options.erase(options.options.begin() + i, options.options.begin() + i + count);
            continue;
        }
        if (g_EXCLUDED_OPTIONS.count(op)) {
            options.options.erase(options.options.begin() + i);
            continue;
        }
        auto it = g_EXCLUDED_OPTIONS_STARTS_WITH.upper_bound(op);
        if (it != g_EXCLUDED_OPTIONS_STARTS_WITH.begin() && StrUtils::StartsWith(op, *--it)) {
            options.options.erase(options.options.begin() + i);
        } else if (StrUtils::StartsWith(op, "--sysroot")) {
            static const size_t equalPos = std::string("--sysroot").size();
            // for clang-cc1 there should be a space symbol between "-isysroot" and "<path>"
            if (auto strLen = op.size(); strLen > equalPos && op[equalPos] == '=') {
                op = op.substr(equalPos + 1);
                options.options.insert(options.options.begin() + i, "-isysroot");
            } else {
                op = StrUtils::Replace(op, "--sysroot", "-isysroot");
            }
        }
    }
}
