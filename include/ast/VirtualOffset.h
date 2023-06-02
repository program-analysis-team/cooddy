/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_VIRTUALOFFSET_H
#define COODDY_VIRTUALOFFSET_H

#include <utils/EnumMapping.h>

#include <cstdint>
namespace HCXX {

class VirtualOffset {
public:
    DECLARE_ENUM(Kind, REF, DEREF, ADDR_OF, OBJECT, INDEX);
    using SubOffsetDescription = std::pair<Kind, uint32_t>;

    explicit VirtualOffset(uint64_t rawOffset = 0) : myOffset(rawOffset) {}
    explicit VirtualOffset(Kind kind, uint32_t index = 0)
    {
        myOffset = static_cast<uint64_t>(kind) + index;
    }

    explicit VirtualOffset(std::string_view subOffsets);

    ~VirtualOffset() = default;

    VirtualOffset& operator+=(const VirtualOffset& virtualOffset)
    {
        for (auto offset = virtualOffset.myOffset; offset != 0; offset >>= 8u) {
            AppendOffset(offset & 0xFFu);
        }
        return *this;
    }

    friend VirtualOffset operator+(const VirtualOffset& lhs, const VirtualOffset& rhs)
    {
        VirtualOffset result(lhs);
        return result += rhs;
    }

    friend VirtualOffset operator-(const VirtualOffset& lhs, const VirtualOffset& rhs);

    bool operator==(const VirtualOffset& offset) const
    {
        return myOffset == offset.myOffset;
    }

    bool operator<(const VirtualOffset& offset) const
    {
        return myOffset < offset.myOffset;
    }

    bool operator!=(const VirtualOffset& offset) const
    {
        return !(*this == offset);
    }

    uint64_t GetRawValue() const
    {
        return myOffset;
    }

    bool IsSourceObject() const
    {
        return myOffset == 0;
    }

    bool IsSubOffsetOf(const VirtualOffset& virtualOffset) const;

    /// Returns true if offset ends with ADDR_OF subOffset
    bool IsAddrOf() const
    {
        return GetLastSubOffset() == static_cast<uint32_t>(Kind::ADDR_OF);
    }

    /// Returns true if offset ends with INDEX subOffset
    bool IsIndex() const
    {
        return GetLastSubOffset() >= static_cast<uint32_t>(Kind::INDEX);
    }

    /// Returns true if offset ends with DEREF subOffset
    bool IsDeref() const
    {
        return GetLastSubOffset() == static_cast<uint32_t>(Kind::DEREF);
    }

    /// Returns true if sub offset can be added
    bool CanAddSubOffset() const
    {
        return myOffset < (1ULL << 56u);
    }

    SubOffsetDescription ExtractSubOffset(bool extractFromBeginning = true)
    {
        uint32_t subOffset = 0;
        if (extractFromBeginning) {
            subOffset = myOffset & 0xFFu;
            myOffset >>= 8u;
        } else if (myOffset != 0) {
            uint32_t shiftCount = 0;
            for (auto offset = myOffset; offset != 0; offset >>= 8u) {
                subOffset = offset & 0xFFu;
                shiftCount++;
            }
            ClearBits(--shiftCount * 8u);
        }

        if (subOffset >= static_cast<uint32_t>(Kind::INDEX)) {
            return {Kind::INDEX, subOffset - static_cast<uint32_t>(Kind::INDEX)};
        }
        return {static_cast<Kind>(subOffset), 0UL};
    }

    std::string ToString();

private:
    uint32_t GetLastSubOffset() const
    {
        uint32_t subOffset = 0;
        for (auto offset = myOffset; offset != 0; offset >>= 8u) {
            subOffset = offset & 0xFFu;
        }
        return subOffset;
    }
    void ClearBits(uint32_t bits)
    {
        myOffset &= (0xffffffffffffffULL >> (56u - bits));
    }

    void AppendOffset(uint64_t offset);

    uint64_t myOffset;
};

}  // namespace HCXX

#endif  // COODDY_VIRTUALOFFSET_H
