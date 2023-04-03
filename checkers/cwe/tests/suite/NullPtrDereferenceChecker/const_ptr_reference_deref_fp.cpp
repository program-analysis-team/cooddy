#include <memory>

struct Iid {
    unsigned int value;
};

struct External {
    uint32_t GetObj()
    {
        return 0;
    }

    uint32_t GetObj(uint32_t expectObjSize, const void*& obj);
};

int main()
{
    const Iid* ptr = nullptr;
    External external;
    (&external)->GetObj(5, (const void*&)ptr);
    return ptr->value;
}
