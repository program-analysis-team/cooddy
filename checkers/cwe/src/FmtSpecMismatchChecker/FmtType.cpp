/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include "FmtType.h"

FmtType FmtType::FromGenericType(HCXX::Type t)
{
    if (!t.IsValid()) {
        return Invalid();
    }

    FmtType result = Invalid();

    if (t.IsPointer()) {
        // Get inner type...
        result = FromGenericType(t.GetPointedType());

        // ..and add a pointer:
        //  - pointers get a pointer and become void** (good enough for correct analysis)
        //  - undetermined or invalid types get a pointer and become void*
        if (result.IsTypedPointer()) {
            return FmtTypes::PP_VOID;
        }

        if (result.type == Type::UNKNOWN) {
            return FmtTypes::P_VOID;
        }

        result.quals |= Quals::POINTER;
        return result;
    }

    uint64_t sizeInBits = t.GetSizeInBits();
    result.bitWidth = std::min<uint64_t>(sizeInBits, 255);
    result.occupiedBitWidth = result.bitWidth;

    if (t.IsIntegralType()) {
        if (t.IsUnsigned()) {
            result.type |= Type::UNSIGNED;
        } else {
            result.type |= Type::SIGNED;
        }
    } else if (t.IsFloatingType()) {
        result.type = Type::FLOAT;
    }

    if (!t.IsConstant()) {
        result.quals |= Quals::MUTABLE;
    }

    return result;
}

// LCOV_EXCL_START
std::ostream& operator<<(std::ostream& os, FmtType obj)
{
    if (obj == FmtType::Invalid()) {
        return os << "<invalid>";
    }

    if (obj.IsVoidPointer()) {
        if (obj.IsTypedPointer()) {
            return os << "<void**>";
        }
        return os << "<void*>";
    }

    os << "<";

    if (obj.IsTypedPointer()) {
        os << "ptr to ";
    }

    if (obj.IsMutable()) {
        os << "mutable ";
    }

    os << int(obj.occupiedBitWidth);

    if (obj.bitWidth != obj.occupiedBitWidth) {
        os << "/" << int(obj.bitWidth);
    }

    os << "-bit ";

    auto sep = "";
    if (obj.IsSigned()) {
        os << "signed";
        sep = "/";
    }
    if (obj.IsUnsigned()) {
        os << sep << "unsigned";
        sep = "/";
    }
    if (obj.IsFloat()) {
        os << sep << "float";
        sep = "/";
    }
    if (obj.IsUnknownType()) {
        os << sep << "unknown-type";
    }

    os << ">";
    return os;
}
// LCOV_EXCL_STOP

// LCOV_EXCL_START
bool FmtType::IsUnknownType() const
{
    return type == Type::UNKNOWN;
}
// LCOV_EXCL_STOP

bool FmtType::IsSigned() const
{
    return bool(type & Type::SIGNED);
}

bool FmtType::IsUnsigned() const
{
    return bool(type & Type::UNSIGNED);
}

bool FmtType::IsFloat() const
{
    return bool(type & Type::FLOAT);
}

bool FmtType::IsVoidPointer() const
{
    return bool(type & Type::VOID_POINTER);
}

bool FmtType::IsTypedPointer() const
{
    return bool(quals & Quals::POINTER);
}

bool FmtType::IsAnyPointer() const
{
    return IsVoidPointer() || IsTypedPointer();
}

bool FmtType::IsMutable() const
{
    return bool(quals & Quals::MUTABLE);
}
