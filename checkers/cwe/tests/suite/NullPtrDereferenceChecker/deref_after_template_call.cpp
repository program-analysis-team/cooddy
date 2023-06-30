#include <memory>

struct MyClass {
    template <class T>
    T* func()
    {
        return new T[10];
    }
};

int main()
{
    int* a = 0;
    MyClass c;
    if (a == 0)
        a = c.func<int>();
    return *a;
}
