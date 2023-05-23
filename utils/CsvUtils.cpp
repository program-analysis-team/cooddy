/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.txt.

#include <utils/CsvUtils.h>

namespace HCXX::CsvUtils {

constexpr static char DELIMITER = ',';
constexpr static char QUOTES = '"';

void WriteValue(std::ofstream& fileStream, std::string_view value)
{
    if (value.empty()) {
        return;
    }
    auto position = value.find("\"", 0);
    bool needQuotes = position != std::string::npos || value.find(DELIMITER) != std::string::npos ||
                      value.find("\n") != std::string::npos;
    if (needQuotes) {
        fileStream << QUOTES;
    }
    if (position != std::string::npos) {
        auto startPosition = 0;
        while (position != std::string::npos) {
            fileStream << value.substr(startPosition, position - startPosition + 1);
            fileStream << QUOTES;
            startPosition = position + 1;
            position = value.find(QUOTES, startPosition);
        }
        fileStream << value.substr(startPosition, value.size() - startPosition);
    } else {
        fileStream << value;
    }
    if (needQuotes) {
        fileStream << QUOTES;
    }
}

void WriteRow(std::ofstream& fileStream, std::initializer_list<const std::string_view> values)
{
    bool first = true;
    for (auto& value : values) {
        if (first) {
            first = false;
        } else {
            fileStream << DELIMITER;
        }
        WriteValue(fileStream, value);
    }
    fileStream << "\n";
}

}  // namespace HCXX::CsvUtils
