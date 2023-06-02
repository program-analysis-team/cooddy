/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <CompilerOptionsList.h>
#include <utils/EnvironmentUtils.h>
#include <utils/LocaleUtils.h>
#include <utils/Log.h>
#include <workspace/CompileCommand.h>

namespace HCXX {

void CompilerOptionsList::ParseResponseFile(const std::string& curDir, const std::string& rspFile,
                                            CompilerOptions& options)
{
    std::filesystem::path filePath(curDir);
    filePath.append(rspFile);
    if (filePath.extension() != ".rsp") {
        return;
    }

    std::ifstream scopeStream(filePath.string());

    std::string commandLine;
    std::getline(scopeStream, commandLine);

    ParseCommandLine(curDir, options, commandLine);
}

std::vector<size_t> GetBeforeQuotesPosAndRemoveServiceSymbols(std::string& command)
{
    std::vector<size_t> quotesEntries;
    for (size_t p = 0; p < command.size(); ++p) {
        if (command[p] == '\\' && (command[p + 1] == '\\' || command[p + 1] == '"')) {
            command.erase(command.begin() + p);
        } else if (command[p] == '"') {
            command.erase(command.begin() + p);
            quotesEntries.emplace_back(--p);
        }
    }
    return quotesEntries;
}

bool PosIsLocatedBetweenQuotes(size_t pos, std::vector<size_t>& quotesEntries)
{
    auto size = quotesEntries.size();
    if (size < 2) {
        return false;
    }
    for (int i = 0; i < size - 1; i += 2) {
        if (pos > quotesEntries[i] && pos <= quotesEntries[i + 1]) {
            return true;
        }
    }
    return false;
}

void CompilerOptionsList::ParseCommandLine(const std::string& curDir, CompilerOptions& options, std::string& command)
{
    std::vector<size_t> quotesEntries = GetBeforeQuotesPosAndRemoveServiceSymbols(command);

    size_t curOffset = 0;
    size_t searchOffset = 0;
    while (curOffset < command.size()) {
        size_t tokenEnd = command.find(' ', searchOffset);
        if (tokenEnd == std::string::npos) {
            tokenEnd = command.size();
        }
        if (PosIsLocatedBetweenQuotes(tokenEnd, quotesEntries)) {
            searchOffset = ++tokenEnd;
            continue;
        }
        std::string arg = command.substr(curOffset, tokenEnd - curOffset);
        if (arg[0] == '@') {
            ParseResponseFile(curDir, arg.substr(1), options);
        } else {
            options.options.emplace_back(arg);
        }
        while (command[tokenEnd] == ' ') {
            tokenEnd++;
        }
        curOffset = tokenEnd;
        searchOffset = tokenEnd;
    }
}

void GetFilesFromOptions(const std::filesystem::path& cmdDir, CompilerOptions& options, std::vector<std::string>& files)
{
    for (int i = static_cast<int>(options.options.size()) - 1; i > 0; --i) {
        if (options[i][0] == '-') {
            continue;
        }
        std::filesystem::path filePath(options[i]);
        if (!filePath.is_absolute()) {
            filePath = cmdDir;
            filePath.append(options[i]);
        }
        std::error_code error;
        if (!std::filesystem::is_regular_file(filePath, error)) {
            continue;
        }
        auto ext = filePath.extension();
        if (ext == ".c" || ext == ".cpp" || ext == ".cc" || ext == ".c++" || ext == ".cxx") {
            files.emplace_back(options[i]);
            options.options.erase(options.options.begin() + i);
        }
    }
}

void MakeRelativePathAbsolute(const std::filesystem::path& cmdDir, std::string& path)
{
    std::filesystem::path filePath(path);
    if (!filePath.is_absolute()) {
        filePath = cmdDir;
        filePath.append(path);
        path = EnvironmentUtils::GetCanonicalPath(filePath.string());
    }
}

void ReplaceRelativeIncludes(const std::filesystem::path& cmdDir, CompilerOptions& options)
{
    for (size_t i = 0; i < options.options.size(); ++i) {
        if (options[i].size() < 2 || options[i][0] != '-') {
            continue;
        }
        if (options[i][1] == 'i' && i != options.options.size() - 1) {
            if (options[i] == "-include") {
                std::filesystem::path absolutePathInclude = cmdDir;
                absolutePathInclude.append(options[i + 1]);
                // FIXME: possible relative path of option -include
                //  could be from path from -I. For now ignoring such cases, until such case will met.
                if (!std::filesystem::exists(absolutePathInclude)) {
                    continue;
                }
            }
            MakeRelativePathAbsolute(cmdDir, options[++i]);
        } else if (options[i] == "-I" && i != options.options.size() - 1) {
            // Make -I option joint to include path
            if (options[i] == "-I") {
                MakeRelativePathAbsolute(cmdDir, options[i + 1]);
                options[i].append(options[i + 1]);
                options.options.erase(options.options.begin() + i + 1);
            }
        } else if (options[i][1] == 'I') {
            std::string includePath = options[i].substr(2);
            MakeRelativePathAbsolute(cmdDir, includePath);
            options[i].replace(2, std::string::npos, includePath);
        }
    }
}

struct SecBCommandDB {
    struct SecBCommand {
        std::string directory;
        std::string compiler;
        std::vector<std::string> arguments;
        std::vector<std::string> files;
        DECLARE_FIELDS("directory", directory, "compiler", compiler, "arguments", arguments, "file", files);
    };
    std::vector<SecBCommand> units;
    DECLARE_FIELDS("compilation_unit", units);
};

std::vector<CompileCommand> LoadSecBCompileDB(const std::filesystem::path& dbPath)
{
    SecBCommandDB commandDB;
    try {
        std::ifstream scopeStream(dbPath.string());
        jsoncpp::parse(commandDB, scopeStream, true);
    }
    // LCOV_EXCL_START
    catch (jsoncpp::ParserError& error) {
        HCXX::Log(HCXX::LogLevel::ERROR) << "compileDB parse error: " << error.what() << ", file: " << dbPath.string()
                                         << "\n";
    }
    // LCOV_EXCL_STOP
    std::vector<CompileCommand> commands;
    for (auto& unit : commandDB.units) {
        if (!unit.arguments.empty()) {
            unit.arguments[0] = unit.compiler;
        }
        CompileCommand command{unit.directory, CompileCommand::ConvertArgumentsToCommand(unit.arguments)};
        for (auto& file : unit.files) {
            command.file = file;
            commands.emplace_back(command);
        }
    }
    return commands;
}

std::vector<CompileCommand> LoadCompileDB(const std::filesystem::path& dbPath)
{
    try {
        std::vector<CompileCommand> commands;
        std::ifstream scopeStream(dbPath.string());
        jsoncpp::parse(commands, scopeStream, true);
        return commands;
    } catch (jsoncpp::ParserError& error) {
        if (error.type() != jsoncpp::ParserError::UnexpectedType) {
            // LCOV_EXCL_START
            HCXX::Log(HCXX::LogLevel::ERROR)
                << "compileDB parse error: " << error.what() << ", file: " << dbPath.string() << "\n";
            return {};
            // LCOV_EXCL_STOP
        }
    }
    // if error.type() is UnexpectedType try load db in SecB format
    return LoadSecBCompileDB(dbPath);
}

static void ApplyCommandLineReplacements(std::string& command, const CompilerOptionsList::Replacements& replacements)
{
    if (replacements.empty()) {
        return;
    }
    for (size_t p = 0; p < command.size(); ++p) {
        for (auto& [pattern, replacement] : replacements) {
            if (command.compare(p, pattern.size(), pattern) == 0) {
                command.replace(p, pattern.size(), replacement);
                p += replacement.size() - 1;
                break;
            }
        }
    }
}

bool CompilerOptionsList::ParseScopeFile(std::filesystem::path scopePath, const Replacements& replacements)
{
    if (std::filesystem::is_directory(scopePath)) {
        scopePath.append(COMPILE_COMMAND_DB);
    }
    if (!std::filesystem::exists(scopePath) || scopePath.filename().extension() != ".json") {
        return false;
    }
    std::vector<CompileCommand> commands = LoadCompileDB(scopePath);
    if (commands.empty()) {
        return false;  // LCOV_EXCL_LINE: HCAT-2779
    }
    for (auto& it : commands) {
        CompilerOptions options;
        std::filesystem::path cmdDir(it.directory);
        if (!cmdDir.is_absolute()) {
            cmdDir = scopePath.parent_path();
            cmdDir.append(it.directory);
        }

        ApplyCommandLineReplacements(it.command, replacements);
        ParseCommandLine(cmdDir.string(), options, it.command);
        if (options.options.empty()) {
            HCXX::Log(HCXX::LogLevel::ERROR) << "Options list is empty" << std::endl;
            continue;
        }

        options.compiler = options[0];
        options.directory = it.directory;

        std::vector<std::string> files;
        GetFilesFromOptions(cmdDir, options, files);
        ReplaceRelativeIncludes(cmdDir, options);

        if (files.empty()) {
            files.emplace_back(it.file);
        }

        for (auto& file : files) {
            MakeRelativePathAbsolute(cmdDir, file);
            myOptions.emplace_back(options)[0] = file;
        }
    }
    SortTUnitsByOptions();
    HCXX::Log(HCXX::LogLevel::INFO) << "Compile commands is loaded: '" << scopePath.string() << "'" << std::endl;
    return true;
}

void CompilerOptionsList::SortTUnitsByOptions()
{
    struct SortIndex {
        size_t orderPos = 0;
        uint32_t bitOption = 64;  // default value
        std::string target;
        const std::string* directory = nullptr;
    };
    std::vector<SortIndex> targetOrder;
    targetOrder.reserve(myOptions.size());
    for (auto& unit : myOptions) {
        auto& order = targetOrder.emplace_back(SortIndex{targetOrder.size()});
        order.directory = &unit.directory;
        for (auto& option : unit.options) {
            if (option == "-m32") {
                order.bitOption = 32;
            }
            if (order.target.empty() && StrUtils::StartsWith(option, "--target=")) {
                order.target = option.substr(9);
            }
        }
    }
    std::stable_sort(targetOrder.begin(), targetOrder.end(), [](auto& o1, auto& o2) {
        return o1.bitOption != o2.bitOption
                   ? o1.bitOption > o2.bitOption
                   : (o1.target != o2.target ? o1.target < o2.target : *o1.directory < *o2.directory);
    });
    std::vector<CompilerOptions> sortedOptions(myOptions.size());
    for (auto i = 0; i < myOptions.size(); ++i) {
        std::swap(sortedOptions[i], myOptions[targetOrder[i].orderPos]);
    }
    std::swap(sortedOptions, myOptions);
}

};  // namespace HCXX
