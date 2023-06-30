#include <map>
struct B {
};

struct C {
    C(const void* v) : value(v){};
    const void* value;
};

struct A {
    const void* GetNode(int id)
    {
        auto& result = myIdMap[id];
        if (result == nullptr) {
            result = new B();
        }
        return result;
    }
    std::map<int, const void*> myIdMap;
};

C getC(A& v,int id) {
    return C(v.GetNode(id));
}

void generator(A& v, int id)
{
    auto c = getC(v, id);
}
