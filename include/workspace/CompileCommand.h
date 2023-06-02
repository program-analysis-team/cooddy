/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
//
// struct CompileCommand declares json structure from compile_command.json
//
#ifndef COODDY_ANALYZER_INCLUDE_COMPILECOMMAND_H_
#define COODDY_ANALYZER_INCLUDE_COMPILECOMMAND_H_

#include <utils/json-cpp.h>

#define COMPILE_COMMAND_DB "compile_commands.json"

namespace HCXX {

struct CompileCommand {
    std::string directory;
    std::string command;
    std::string file;
    std::string output;

    static std::string ConvertArgumentsToCommand(const std::vector<std::string>& arguments)
    {
        std::string command;
        for (auto& str : arguments) {
            command += '"';
            for (auto it : str) {
                if (it == '\\' || it == '"') {
                    command += "\\";
                }
                command += it;
            }
            command += "\" ";
        }
        return command;
    }

    template <class X>
    // TODO HCAT-513
    // COODDY_SUPPRESS CamelCaseChecker
    void serialize(jsoncpp::Stream<X>& p)
    {
        std::vector<std::string> arguments;
        jsoncpp::fields(*this, p, "directory", directory, "command", command, "file", file, "output", output,
                        "arguments", arguments);
        if (command.empty() && arguments.size()) {
            command = ConvertArgumentsToCommand(arguments);
        }
    }
};

};      // namespace HCXX
#endif  // COODDY_ANALYZER_INCLUDE_COMPILECOMMAND_H_
