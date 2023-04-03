#include <cstdint>

struct Iid {};

struct ColData {
    int col;
};

uint32_t Get(const Iid &iid, const void *&obj, uint32_t &objSize) {
    if (objSize == 4) {
        auto data = ColData{4};
        obj = &data;
    }
    return 0;
}

namespace Gof {
class Roc {
public:
    uint32_t GetObj(const Iid& iid, const void*& obj, uint32_t& objSize) const
    {
        uint32_t ret = Get(iid, obj, objSize);
        return ret;
    }

    uint32_t GetObj(const Iid& iid, uint32_t expectObjSize, const void*& obj) const
    {
        uint32_t objSize = 4;
        uint32_t ret = GetObj(iid, obj, objSize);
        if (ret == 0) {
            return (objSize == expectObjSize) ? 0 : 10;
        }
        return ret;
    }
};
}

class Singleton : public Gof::Roc {
public:
    static Singleton *GetInstance() {
        static Singleton *instance = nullptr;
        if (instance == nullptr) {
            instance = new Singleton;
        }
        return instance;
    }
};

uint32_t Foo(const Iid &iid, const ColData *&data) {
    return Singleton::GetInstance()->GetObj(iid, sizeof(ColData),
                                            (const void *&)data);
}

int main() {
    const ColData *coL1Data = nullptr;
    Iid iid{};
    Foo(iid, coL1Data);
    return coL1Data->col;
}
