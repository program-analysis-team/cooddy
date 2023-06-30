#include <map>
struct B {
};

struct A {
    const void* GetNode(int id)
    {
        auto& result = myIdMap[id] ;
        if (result == nullptr) {
            result = new B();
        }
        return result;
    }
    std::map<int, const void*> myIdMap;
};

void generator(A& v, int id)
{
    auto node = v.GetNode(id);
}
