#include <map>
struct B {
};

struct A {
    void AddIdMapping(int id, const void* node)
    {
        myIdMap[id] = node;
    }
    std::map<int, const void*> myIdMap;
};

void generator(A& v, int id)
{
    auto ptr = new B();
    v.AddIdMapping(id, ptr);
}
