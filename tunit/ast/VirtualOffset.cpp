/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <ast/VirtualOffset.h>
#include <utils/LocaleUtils.h>
#include <utils/Log.h>

namespace HCXX {

VirtualOffset::VirtualOffset(std::string_view subOffsets)
{
    myOffset = 0;
    for (size_t i = 0; i < subOffsets.size(); i++) {
        uint64_t offset = 0;
        if (subOffsets[i] == '[') {
            auto pos = subOffsets.find(']', i);
            if (pos == std::string::npos) {
                // LCOV_EXCL_START
                Log(LogLevel::ERROR) << "Failed to parse offset\n";
                break;
                // LCOV_EXCL_STOP
            }
            std::string index(subOffsets, i + 1, pos - i - 1);
            offset = static_cast<uint64_t>(Kind::INDEX) + std::stoul(index);
            i = pos;
        } else if (subOffsets[i] == '*') {
            offset = static_cast<uint64_t>(Kind::DEREF);
        } else if (subOffsets[i] == '&') {
            offset = static_cast<uint64_t>(Kind::ADDR_OF);
        } else if (subOffsets[i] == 'o') {
            myOffset = static_cast<uint64_t>(Kind::OBJECT);
            continue;
        }
        AppendOffset(offset);
    }
}

std::string VirtualOffset::ToString()
{
    std::string value;

    for (auto offset = *this; !offset.IsSourceObject();) {
        auto subOffset = offset.ExtractSubOffset();
        switch (subOffset.first) {
            case Kind::INDEX:
                value += "[" + std::to_string(subOffset.second) + "]";
                break;
            case Kind::DEREF:
                value += "*";
                break;
            case Kind::ADDR_OF:
                value += "&";
                break;
            case Kind::OBJECT:
                value += "o";
                break;
                // LCOV_EXCL_START: HCAT-2777
            default:
                break;
                // LCOV_EXCL_STOP
        }
    }
    return value;
}

VirtualOffset operator-(const VirtualOffset& lhs, const VirtualOffset& rhs)
{
    // Note: lhs must be suboffset of rhs, in other cases result isn't correct
    VirtualOffset result(lhs);
    if (result.IsSourceObject()) {
        return result;
    }

    auto rhsOffset = rhs.myOffset;
    while (rhsOffset) {
        result.myOffset >>= 8u;
        rhsOffset >>= 8u;
    }
    return result;
}

bool VirtualOffset::IsSubOffsetOf(const VirtualOffset& virtualOffset) const
{
    if (virtualOffset.IsSourceObject()) {
        return true;
    }

    auto subOffset = myOffset;
    auto parentOffset = virtualOffset.myOffset;

    if (subOffset < parentOffset) {
        return false;
    }

    while (parentOffset) {
        if ((subOffset & 0xFFu) != (parentOffset & 0xFFu)) {
            return false;
        }
        subOffset >>= 8u;
        parentOffset >>= 8u;
    }

    return true;
}

void VirtualOffset::AppendOffset(uint64_t offset)
{
    uint32_t bits = 0;
    if (offset == static_cast<uint64_t>(Kind::OBJECT)) {
        for (uint64_t curOffset = myOffset; curOffset != 0; bits += 8u, curOffset >>= 8u) {
            if ((curOffset & 0xFFu) < static_cast<uint64_t>(Kind::INDEX)) {
                break;
            }
        }
        if (bits != 0) {
            uint64_t mask = (1ULL << (bits - 8u)) - 1u;
            myOffset = (myOffset & mask) | ((myOffset >> 8u) & ~mask);
            return;
        }
    }
    for (uint64_t curOffset = myOffset; curOffset != 0; bits += 8u, curOffset >>= 8u) {
        if (offset >= static_cast<uint64_t>(Kind::INDEX) && curOffset < static_cast<uint64_t>(Kind::INDEX)) {
            ClearBits(bits);
            break;
        }
        if (offset == static_cast<uint64_t>(Kind::ADDR_OF) && curOffset == static_cast<uint64_t>(Kind::DEREF) ||
            offset == static_cast<uint64_t>(Kind::DEREF) && curOffset == static_cast<uint64_t>(Kind::ADDR_OF)) {
            ClearBits(bits);
            return;
        }
    }
    myOffset |= (offset << bits);
}

}  // namespace HCXX
