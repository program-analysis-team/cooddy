/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <ast/Node.h>

#include "TestBaseClass.h"

// The fixture for testing class EnumConversionsTest
class EnumConversionsTest : public TestBaseClass {};

TEST_F(EnumConversionsTest, NodeKindToStr)
{
    ::HCXX::Node::Kind kind = ::HCXX::Node::Kind::REF_EXPRESSION;
    ASSERT_STREQ(::HCXX::Node::KindToCStr(kind), "RefExpression");
}

TEST_F(EnumConversionsTest, NodeKindFromStr)
{
    const char str[] = "TryExpression";
    ASSERT_EQ(::HCXX::Node::KindFromStr(str), ::HCXX::Node::Kind::TRY_EXPRESSION);
}

TEST_F(EnumConversionsTest, UnknownNodeKindFromStr)
{
    const char str[] = "SomeStrangeText";
    ASSERT_EQ(::HCXX::Node::KindFromStr(str), ::HCXX::Node::Kind::NODE);
}

TEST_F(EnumConversionsTest, NodeKindFromStrWithCheck)
{
    const char str[] = "ThrowExpression";
    std::optional<::HCXX::Node::Kind> res = ::HCXX::Node::KindFromStrWithCheck(str);
    ASSERT_TRUE(res.has_value());
    ASSERT_EQ(res.value(), ::HCXX::Node::Kind::THROW_EXPRESSION);
}

TEST_F(EnumConversionsTest, UnknownNodeKindFromStrWithCheck)
{
    const char str[] = "SomeStrangeText";
    std::optional<::HCXX::Node::Kind> res = ::HCXX::Node::KindFromStrWithCheck(str);
    ASSERT_FALSE(res.has_value());
}
