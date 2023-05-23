/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.txt.
#ifndef CSV_UTILS_HEADER_FILE
#define CSV_UTILS_HEADER_FILE

#include <fstream>

namespace HCXX::CsvUtils {

void WriteValue(std::ofstream& fileStream, std::string_view value);

void WriteRow(std::ofstream& fileStream, std::initializer_list<const std::string_view> values);

}  // namespace HCXX::CsvUtils

#endif
