/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_TYPE_H
#define COODDY_TYPE_H

#include <memory.h>
#include <utils/IOStream.h>

#include <cstdint>

class ASTConverter;

namespace HCXX {

class Node;

class Type {
public:
    bool IsValid() const
    {
        return GetSizeInBits() != 0;
    }

    bool IsTrivial() const
    {
        return IsValid() && !myPointerFlags && !myIsDeclaration && !myIsArray;
    }

    bool IsUnsigned() const
    {
        return !myPointerFlags && myIsUnsigned;
    }

    bool IsConstant() const
    {
        return myConstantFlags & 1u;
    }

    bool IsPointer() const
    {
        return myPointerFlags > 0;
    }

    bool IsReference() const
    {
        return myReferenceFlags & 1u;
    }

    bool IsDeclaration() const
    {
        return myIsDeclaration;
    }

    bool IsVariableArray() const
    {
        return myIsVariableArray;
    }

    bool IsConstantArray() const
    {
        return myIsConstantArray;
    }

    bool IsArray() const
    {
        return myIsArray;
    }

    bool IsFloatingType() const
    {
        return myIsFloatingType;
    }

    bool IsIntegralType() const
    {
        return myIsUnsigned || myIsIntegerType;
    }

    bool IsPointedToConstant() const
    {
        uint8_t pointerFlags = this->myPointerFlags;
        uint8_t constantFlags = this->myConstantFlags;
        while (pointerFlags-- > 0) {
            constantFlags >>= 1u;
            pointerFlags >>= 1u;
        }
        return constantFlags & 1u;
    }

    bool IsRvalueReferenceType() const
    {
        return myIsRvalueType;
    }

    bool IsSensitiveData() const
    {
        return myIsSensitiveData;
    }

    Type GetPointedType() const
    {
        Type result(*this);
        result.myPointerFlags--;
        result.myReferenceFlags >>= 1u;
        result.myConstantFlags >>= 1u;
        return result;
    }

    const Node* GetDeclaration() const
    {
        return !myPointerFlags && myIsDeclaration ? myDeclNode : nullptr;
    }

    const Node* GetPointedDeclaration() const
    {
        return myIsDeclaration ? myDeclNode : nullptr;
    }

    uint64_t GetSizeInBits() const
    {
        return !myPointerFlags ? mySizeInBits : sizeof(uintptr_t) * 8;
    }

    void SetSizeInBits(uint64_t sizeInBits)
    {
        mySizeInBits = sizeInBits;
    }
    std::string GetPrintableName() const;
    std::string Dump() const;

    bool operator==(const Type& other) const
    {
        return memcmp(this, &other, sizeof(*this)) == 0;
    }

    bool operator!=(const Type& other) const
    {
        return !operator==(other);
    }

    void Serialize(IOStream& stream)
    {
        // COODDY_SUPPRESS
        stream.SerializePod(*reinterpret_cast<uint64_t*>(&mySizeInBits));
        if (myIsDeclaration) {
            stream << myDeclNode;
        }
    }

    ~Type() = default;

    Type()
        : myDeclNode(nullptr),
          mySizeInBits(0),
          myIsUnsigned(0),
          myIsDeclaration(0),
          myPointerFlags(0),
          myReferenceFlags(0),
          myConstantFlags(0),
          myIsVariableArray(0),
          myIsConstantArray(0),
          myIsArray(0),
          myIsFloatingType(0),
          myIsIntegerType(0),
          myIsRvalueType(0),
          myIsSensitiveData(0)
    {}

private:
    const Node* myDeclNode;
    uint32_t mySizeInBits;
    uint8_t myReferenceFlags;
    uint8_t myConstantFlags;
    uint8_t myPointerFlags : 7;  // 0-8 number of bits in myReferenceFlags or myConstantFlags
    uint8_t myIsUnsigned : 1;
    uint8_t myIsDeclaration : 1;
    uint8_t myIsVariableArray : 1;
    uint8_t myIsConstantArray : 1;
    uint8_t myIsArray : 1;
    uint8_t myIsFloatingType : 1;
    uint8_t myIsIntegerType : 1;
    uint8_t myIsRvalueType : 1;
    uint8_t myIsSensitiveData : 1;

    friend class ASTVisitor;
    friend class ::ASTConverter;
};

static_assert(sizeof(Type) <= 16, "The size of Type is too large");

}  // namespace HCXX

#endif  // COODDY_TYPE_H
