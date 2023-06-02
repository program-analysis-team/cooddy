/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <mutex>
#include <unordered_map>

class StringsMapping {
public:
    uint32_t GetStringId(std::string_view s)
    {
        auto stringCRC = CalcCRC16(s.data(), s.size());
        std::lock_guard<std::mutex> lock(myMutex);
        for (uint32_t i = 0;; i++) {
            uint32_t stringId = (i << 16u) | stringCRC;
            auto& mapVal = myMapping.emplace(stringId, s).first->second;
            if (mapVal == s) {
                return stringId;
            }
        }
        return 0;
    }

    const std::string& GetStringById(uint32_t stringId)
    {
        std::lock_guard<std::mutex> lock(myMutex);
        return myMapping[stringId];
    }

private:
    static uint16_t CalcCRC16(const char* data, uint32_t length)
    {
        uint16_t crc = 0x1D0F;
        while (length--) {
            uint8_t x = crc >> 8 ^ uint8_t(*data++);
            x ^= x >> 4;
            crc = (crc << 8) ^ uint16_t(x << 12) ^ uint16_t(x << 5) ^ uint16_t(x);
        }
        return crc;
    }

private:
    std::mutex myMutex;
    std::unordered_map<uint32_t, std::string> myMapping;
};
