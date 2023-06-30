#include <memory>

struct MyClass {
    MyClass(const char* p) {}
    int var = 10;
};

MyClass* Create(const char* p)
{
    if (p == nullptr)
        return nullptr;
    MyClass* ret = new MyClass(p);
    ret->var = 15;
    return ret;
}

int main()
{
    Create("Hello");
    return 0;
}
