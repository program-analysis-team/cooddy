/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_SOURCE_CHECKERS_CWE_FMTSPECMISMATCHCHECKER_TYPE_H_
#define COODDY_ANALYZER_SOURCE_CHECKERS_CWE_FMTSPECMISMATCHCHECKER_TYPE_H_
#include <ast/Type.h>
#include <utils/EnumMapping.h>

#include <array>
#include <ostream>
#include <string_view>
#include <tuple>

/// Complete type information about a printf-style format argument.
class FmtType {
public:
    // clang-format off
    enum class Type : uint8_t {
        UNKNOWN      = 0,      // For invalid format specifiers or unknown types.
        SIGNED       = 0b0001,
        UNSIGNED     = 0b0010,
        FLOAT        = 0b0100,
        VOID_POINTER = 0b1000  // void* is treated as a self-contained type, instead of a pointer to something.
    };

    enum class Quals : uint8_t {
        NONE     = 0,
        POINTER  = 0b01,
        MUTABLE  = 0b10,
    };
    // clang-format on

    Type type;
    Quals quals;

    uint8_t bitWidth;
    /// Size in bits occupied by set bits (when value is known, otherwise set to bitWidth).
    uint8_t occupiedBitWidth;

    constexpr FmtType(Type type, uint8_t bitWidth) : FmtType(type, Quals::NONE, bitWidth, bitWidth) {}
    constexpr FmtType(Type type, uint8_t bitWidth, uint8_t occupiedBitWidth)
        : FmtType(type, Quals::NONE, bitWidth, occupiedBitWidth)
    {}
    constexpr FmtType(Type type, Quals quals, uint8_t bitWidth) : FmtType(type, quals, bitWidth, bitWidth) {}
    constexpr FmtType(Type type, Quals quals, uint8_t bitWidth, uint8_t occupiedBitWidth)
        : type(type), quals(quals), bitWidth(bitWidth), occupiedBitWidth(occupiedBitWidth)
    {}

    /// Convert a generic type to a matching FmtFmtArgType.
    static FmtType FromGenericType(HCXX::Type t);

    /// Convert an actual comiler type T to a matching FmtFmtArgType.
    /// Since const-ness is default in this abstraction, constness of the type T is ignored.
    template <typename T>
    static constexpr FmtType FromTypesystem()
    {
        FmtType result = FmtType::Invalid();

        if constexpr (std::is_pointer_v<T>) {
            result = FromTypesystem<std::remove_pointer_t<T>>();
            result.quals = Quals::POINTER;
            return result;
        }

        result.type = std::is_integral_v<T> ? std::is_signed_v<T> ? Type::SIGNED : Type::UNSIGNED : Type::FLOAT;
        result.bitWidth = sizeof(T) * 8;
        result.occupiedBitWidth = sizeof(T) * 8;

        return result;
    }

    /// Get a null-like invalid value.
    static constexpr FmtType Invalid()
    {
        return FmtType{Type::UNKNOWN, 0};
    }

    friend bool operator==(const FmtType& lhs, const FmtType& rhs)
    {
        return std::tie(lhs.type, lhs.quals, lhs.bitWidth, lhs.occupiedBitWidth) ==
               std::tie(rhs.type, rhs.quals, rhs.bitWidth, rhs.occupiedBitWidth);
    }

    friend bool operator!=(const FmtType& lhs, const FmtType& rhs)
    {
        return !(lhs == rhs);
    }

    explicit operator bool() const
    {
        return *this != Invalid();
    }

    friend std::ostream& operator<<(std::ostream& os, FmtType obj);

    bool IsUnknownType() const;
    bool IsSigned() const;
    bool IsUnsigned() const;
    bool IsFloat() const;
    bool IsVoidPointer() const;
    bool IsTypedPointer() const;
    bool IsAnyPointer() const;
    bool IsMutable() const;
};

DEFINE_ENUM_BITWISE_OPERATIONS(FmtType::Type);
DEFINE_ENUM_BITWISE_OPERATIONS(FmtType::Quals);

namespace FmtTypes {

constexpr FmtType INV = FmtType::Invalid();

constexpr FmtType CHAR = FmtType::FromTypesystem<char>();
constexpr FmtType WCHAR = FmtType::FromTypesystem<wchar_t>();

constexpr FmtType ICHAR = FmtType::FromTypesystem<signed char>();
constexpr FmtType SHORT = FmtType::FromTypesystem<short>();
constexpr FmtType INT = FmtType::FromTypesystem<int>();
constexpr FmtType LONG = FmtType::FromTypesystem<long int>();
constexpr FmtType LLONG = FmtType::FromTypesystem<long long int>();
constexpr FmtType ISIZE = FmtType::FromTypesystem<std::ptrdiff_t>();

constexpr FmtType UCHAR = FmtType::FromTypesystem<unsigned char>();
constexpr FmtType USHORT = FmtType::FromTypesystem<unsigned short>();
constexpr FmtType UINT = FmtType::FromTypesystem<unsigned int>();
constexpr FmtType ULONG = FmtType::FromTypesystem<unsigned long int>();
constexpr FmtType ULLONG = FmtType::FromTypesystem<unsigned long long int>();
constexpr FmtType USIZE = FmtType::FromTypesystem<std::size_t>();

constexpr FmtType F32 = FmtType::FromTypesystem<float>();
constexpr FmtType F64 = FmtType::FromTypesystem<double>();
constexpr FmtType FMAX = FmtType::FromTypesystem<long double>();

constexpr FmtType P_CHAR = FmtType::FromTypesystem<char*>();
constexpr FmtType P_WCHAR = FmtType::FromTypesystem<wchar_t*>();
constexpr FmtType P_VOID = {FmtType::Type::VOID_POINTER, 0};
constexpr FmtType PP_VOID = {FmtType::Type::VOID_POINTER, FmtType::Quals::POINTER, 0};

}  // namespace FmtTypes

#endif  // COODDY_ANALYZER_SOURCE_CHECKERS_CWE_FMTSPECMISMATCHCHECKER_TYPE_H_
