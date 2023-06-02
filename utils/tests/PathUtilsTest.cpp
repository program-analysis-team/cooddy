/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <cstddef>
#include <string_view>
#include <vector>

#include "TestBaseClass.h"
#include "utils/PathUtils.h"

using namespace HCXX::PathUtils;

// The fixture for testing class StrUtilsTests
class PathUtilsTest : public TestBaseClass {};

TEST_F(PathUtilsTest, FileUriWindows)
{
    ASSERT_EQ(FileUri("C:\\Program Files\\LLVM\\bin\\clang.exe", PathFormat::WINDOWS),
              "file:///C:/Program Files/LLVM/bin/clang.exe");

    // Relative files: forbidden by URI spec but handled by many interpreters
    ASSERT_EQ(FileUri("sdk\\utils/tests\\PathUtilsTest.cpp", PathFormat::WINDOWS),
              "file://sdk/utils/tests/PathUtilsTest.cpp");

    // UNC paths: currently not supported
    // ASSERT_EQ(FileUri("\\\\localhost\\example.cpp"), "file://localhost/example.cpp");
}

TEST_F(PathUtilsTest, FileUriPosix)
{
    ASSERT_EQ(FileUri("/etc/manifesto/为党争光", PathFormat::POSIX),
              "file:///etc/manifesto/%e4%b8%ba%e5%85%9a%e4%ba%89%e5%85%89");

    // Relative files: forbidden by URI spec but handled by many interpreters
    ASSERT_EQ(FileUri("sdk/utils/tests/PathUtilsTest.cpp", PathFormat::POSIX),
              "file://sdk/utils/tests/PathUtilsTest.cpp");
}
