/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_SOURCE_SOLVER_INSTRUCTIONSET_H_
#define COODDY_ANALYZER_SOURCE_SOLVER_INSTRUCTIONSET_H_

#include <cstdint>
#include <cstring>
#include <string_view>
#include <vector>

class DataStream {
public:
    template <class T>
    void Add(T val)
    {
        uint8_t* ptr = reinterpret_cast<uint8_t*>(&val);
        myInstr.insert(myInstr.end(), ptr, ptr + sizeof(T));
    }

    void AddString(std::string_view str)
    {
        myInstr.reserve(myInstr.size() + str.size() + 1);
        myInstr.insert(myInstr.end(), str.data(), str.data() + str.size());
        myInstr.push_back(0);  // null terminator
    }

    template <class T>
    T Get(uint32_t* ppos = nullptr)
    {
        uint32_t& pos = ppos ? *ppos : myPos;
        T result = *reinterpret_cast<T*>(&myInstr[pos]);
        pos += sizeof(T);
        return result;
    }

    template <class T>
    T* GetPtr(uint32_t* ppos = nullptr)
    {
        uint32_t& pos = ppos ? *ppos : myPos;
        T* result = reinterpret_cast<T*>(&myInstr[pos]);
        pos += sizeof(T);
        return result;
    }

    const char* GetString(uint32_t* ppos = nullptr)
    {
        uint32_t& pos = ppos ? *ppos : myPos;
        const char* result = reinterpret_cast<const char*>(myInstr.data()) + pos;
        pos += std::strlen(result) + 1;
        return result;
    }

    template <class T = uint8_t>
    void GetBuffer(T* buffer, uint32_t size, uint32_t* ppos = nullptr)
    {
        uint32_t& pos = ppos ? *ppos : myPos;
        std::memcpy(buffer, &myInstr[pos], size * sizeof(T));
        pos += size * sizeof(T);
    }

    template <class T = uint8_t>
    void AddBuffer(const T* buffer, uint32_t size)
    {
        auto ptr = reinterpret_cast<const uint8_t*>(buffer);
        myInstr.insert(myInstr.end(), ptr, ptr + size * sizeof(T));
    }

    uint32_t Count() const
    {
        return myInstr.size();
    }

    template <class T = uint8_t>
    T* GetData(uint32_t size = 0)
    {
        // LCOV_EXCL_START
        if (size != 0) {
            myInstr.resize(size * sizeof(T));
        }
        // LCOV_EXCL_STOP
        return reinterpret_cast<T*>(myInstr.data());
    }

    bool IsEOF() const
    {
        return myPos >= myInstr.size();
    }

    uint32_t GetCurPos() const
    {
        return myPos;
    }

private:
    std::vector<uint8_t> myInstr;
    uint32_t myPos = 0;
};

#endif  // COODDY_ANALYZER_SOURCE_SOLVER_INSTRUCTIONSET_H_
