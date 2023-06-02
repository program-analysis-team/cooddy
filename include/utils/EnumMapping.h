/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
//
// Macro definitions for enum declaration with string mapping
//
#ifndef COODDY_ANALYZER_INCLUDE_ENUMMAPPING_H_
#define COODDY_ANALYZER_INCLUDE_ENUMMAPPING_H_

#include <utils/StrUtils.h>

#include <optional>
#include <ostream>
#include <string_view>
#include <type_traits>
#include <vector>

namespace HCXX {

template <class EnumType>
class EnumMapping {
public:
    explicit EnumMapping(std::string_view str)
    {
        for (std::string_view::size_type pos = 0; pos < str.size(); /* Do nothing */) {
            auto commaPos = str.find_first_of(",", pos);
            auto tmp = HCXX::StrUtils::TrimWhitespace(str.substr(pos, commaPos - pos));
            if (!tmp.empty()) {
                myMapping.emplace_back(HCXX::StrUtils::BigSnakeCaseToCamelCase(std::move(tmp)));
            }
            pos = (commaPos == std::string_view::npos) ? str.size() : commaPos + 1;
        }
    }
    // LCOV_EXCL_START
    ~EnumMapping() = default;

    const char* GetNameByValue(EnumType value) const
    {
        return static_cast<size_t>(value) < myMapping.size() ? myMapping[static_cast<size_t>(value)].c_str() : nullptr;
    }
    // LCOV_EXCL_STOP

    std::optional<EnumType> GetValueByName(const std::string_view name) const
    {
        for (size_t i = 0; i < myMapping.size(); ++i) {
            if (!name.compare(myMapping[i])) {
                return static_cast<EnumType>(i);
            }
        }
        return std::nullopt;
    }

private:
    std::vector<std::string> myMapping;
};

};  // namespace HCXX

#define DECLARE_ENUM(EnumName, ...)                                                              \
    enum class EnumName { __VA_ARGS__ };                                                         \
    static const EnumMapping<EnumName>& Get##EnumName##Mapping()                                 \
    {                                                                                            \
        static const char names[] = #__VA_ARGS__;                                                \
        static EnumMapping<EnumName> mapping(names);                                             \
        return mapping;                                                                          \
    }                                                                                            \
    static const char* EnumName##ToCStr(EnumName value)                                          \
    {                                                                                            \
        return Get##EnumName##Mapping().GetNameByValue(value);                                   \
    }                                                                                            \
    static std::optional<EnumName> EnumName##FromStrWithCheck(const std::string_view kind)       \
    {                                                                                            \
        return Get##EnumName##Mapping().GetValueByName(kind);                                    \
    }                                                                                            \
    static EnumName EnumName##FromStr(const std::string_view kind)                               \
    {                                                                                            \
        return Get##EnumName##Mapping().GetValueByName(kind).value_or(static_cast<EnumName>(0)); \
    }

#define DEFINE_ENUM_BITWISE_OPERATIONS(Enum)                                                 \
    constexpr Enum operator|(Enum lhs, Enum rhs) noexcept                                    \
    {                                                                                        \
        using base_type = std::underlying_type_t<Enum>;                                      \
        return static_cast<Enum>(static_cast<base_type>(lhs) | static_cast<base_type>(rhs)); \
    }                                                                                        \
                                                                                             \
    constexpr Enum operator&(Enum lhs, Enum rhs) noexcept                                    \
    {                                                                                        \
        using base_type = std::underlying_type_t<Enum>;                                      \
        return static_cast<Enum>(static_cast<base_type>(lhs) & static_cast<base_type>(rhs)); \
    }                                                                                        \
                                                                                             \
    constexpr Enum operator^(Enum lhs, Enum rhs) noexcept                                    \
    {                                                                                        \
        using base_type = std::underlying_type_t<Enum>;                                      \
        return static_cast<Enum>(static_cast<base_type>(lhs) ^ static_cast<base_type>(rhs)); \
    }                                                                                        \
                                                                                             \
    constexpr Enum& operator|=(Enum& lhs, Enum rhs) noexcept                                 \
    {                                                                                        \
        lhs = lhs | rhs;                                                                     \
        return lhs;                                                                          \
    }                                                                                        \
                                                                                             \
    constexpr Enum& operator&=(Enum& lhs, Enum rhs) noexcept                                 \
    {                                                                                        \
        lhs = lhs & rhs;                                                                     \
        return lhs;                                                                          \
    }                                                                                        \
                                                                                             \
    constexpr Enum& operator^=(Enum& lhs, Enum rhs) noexcept                                 \
    {                                                                                        \
        lhs = lhs ^ rhs;                                                                     \
        return lhs;                                                                          \
    }                                                                                        \
                                                                                             \
    constexpr Enum operator~(Enum val) noexcept                                              \
    {                                                                                        \
        using base_type = std::underlying_type_t<Enum>;                                      \
        return static_cast<Enum>(~static_cast<base_type>(val));                              \
    }                                                                                        \
                                                                                             \
    static_assert(std::is_enum_v<Enum>, "DEFINE_ENUM_BITWISE_OPERATIONS used on a non-enum type")

#endif  // COODDY_ANALYZER_INCLUDE_ENUMMAPPING_H_
