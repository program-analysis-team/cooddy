/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_INCLUDE_IOSTREAM_H_
#define COODDY_ANALYZER_INCLUDE_IOSTREAM_H_

#include <utils/DataStream.h>

#include <bitset>
#include <memory>
#include <string>
#include <unordered_map>

namespace HCXX {

class IOStream;

template <class T>
inline auto Serialize(IOStream& stream, T& value) -> decltype(value.Serialize(stream), void())
{
    value.Serialize(stream);
}

class IOStream : public DataStream {
public:
    explicit IOStream(bool isOut) : myIsOut(isOut) {}
    ~IOStream() = default;

    bool IsOut() const
    {
        return myIsOut;
    }

    void AddPtr(const void* node)
    {
        DataStream::Add<uint32_t>(myPtrMap[node]);
    }

    template <class T = const void*>
    T GetPtr()
    {
        // COODDY_SUPPRESS
        return (T)(myIdMap[DataStream::Get<uint32_t>()]);
    }

    void operator<<(std::nullptr_t) {}

    template <class T>
    IOStream& operator<<(T& arr)
    {
        Serialize(*this, arr);
        return *this;
    }

    template <class T>
    void SerializePod(T& value)
    {
        if (IsOut()) {
            Add<T>(value);
        } else {
            value = Get<T>();
        }
    }

    template <class T>
    friend typename std::enable_if<std::is_arithmetic<T>::value>::type Serialize(IOStream& stream, T& value)
    {
        stream.SerializePod(value);
    }

    template <class T>
    friend typename std::enable_if<std::is_enum<T>::value>::type Serialize(IOStream& stream, T& value)
    {
        if (stream.IsOut()) {
            stream.Add<uint8_t>(static_cast<uint8_t>(value));
        } else {
            value = static_cast<T>(stream.Get<uint8_t>());
        }
    }

    template <class T>
    friend typename std::enable_if<std::is_pointer<T>::value>::type Serialize(IOStream& stream, T& value)
    {
        if (stream.IsOut()) {
            stream.AddPtr(value);
        } else {
            value = stream.GetPtr<T>();
        }
    }

    template <class T>
    friend void Serialize(IOStream& stream, std::shared_ptr<T>& value)
    {
        Serialize(stream, *value);
    }

    template <class T>
    friend void Serialize(IOStream& stream, std::vector<T>& value)
    {
        if (stream.IsOut()) {
            stream.Add<uint32_t>(value.size());
        } else {
            value.resize(stream.Get<uint32_t>());
        }
        for (auto& it : value) {
            Serialize(stream, it);
        }
    }

    friend void Serialize(IOStream& stream, std::string& value)
    {
        if (stream.IsOut()) {
            stream.AddString(value.c_str());
        } else {
            value = stream.GetString();
        }
    }

    template <size_t N>
    friend void Serialize(IOStream& stream, std::bitset<N>& value)
    {
        if (stream.IsOut()) {
            stream.Add<uint32_t>(value.to_ulong());
        } else {
            value = std::bitset<N>(stream.Get<uint32_t>());
        }
    }

    template <class TKey, class TVal>
    friend void Serialize(IOStream& stream, std::unordered_map<TKey, TVal>& value)
    {
        TKey key;
        if (stream.IsOut()) {
            stream.Add<uint32_t>(value.size());
            for (auto& it : value) {
                Serialize(stream, key = it.first);
                Serialize(stream, it.second);
            }
        } else {
            auto size = stream.Get<uint32_t>();
            for (auto i = 0; i < size; ++i) {
                Serialize(stream, key);
                Serialize(stream, value[key]);
            }
        }
    }

    void AddPtrMapping(const void* node, uint32_t id)
    {
        myPtrMap[node] = id;
    }

    void AddIdMapping(uint32_t id, const void* node)
    {
        myIdMap[id] = node;
    }

private:
    bool myIsOut;
    std::unordered_map<const void*, uint32_t> myPtrMap;
    std::unordered_map<uint32_t, const void*> myIdMap;
};

};  // namespace HCXX

#endif  // COODDY_ANALYZER_INCLUDE_IOSTREAM_H_
