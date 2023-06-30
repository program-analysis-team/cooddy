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

void init(A& v,const B*& node, int id) {
    node = (const B*)v.GetNode(id);
}

void generator(A& v)
{
    const B* node = nullptr;
    init(v,node, 123);
}
