/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_TESTSTREAM_HEADER_FILE
#define COODDY_TESTSTREAM_HEADER_FILE

#include "TestErrorCheckConsumer.h"

using Traits = std::char_traits<char>;
class TestStreamBuf : public std::basic_streambuf<char, Traits> {
public:
    int overflow(int c) override
    {
        if (!Traits::eq_int_type(c, EOF)) {
            if (auto curTest = TestErrorCheckConsumer::GetCurrentTest(); curTest) {
                curTest->AddTraceSymbol(static_cast<char>(c));
            } else {
                std::cout << static_cast<char>(c);
            }
        }
        return Traits::not_eof(c);
    }
};

class TestStream : public std::basic_ostream<char, std::char_traits<char>> {
    TestStreamBuf myBuffer;

public:
    TestStream() : std::basic_ostream<char, std::char_traits<char>>::basic_ostream(&myBuffer)
    {
        this->init(&myBuffer);
    }
    ~TestStream() override = default;
};

#endif  // #ifndef COODDY_TESTSTREAM_HEADER_FILE
