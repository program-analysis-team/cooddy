#include <memory.h>

void func1();

struct MyClass {
    void func5()
    {
        memset(0, 0, 10);
    }
};

namespace MyNamespace {
void func6()
{
    MyClass().func5();
}
};  // namespace MyNamespace

void func4()
{
    MyNamespace::func6();
    func1();
}

void func3()
{
    MyClass().func5();
    MyNamespace::func6();
}

void MyFunc2()
{
    MyClass().func5();
    func4();
    MyNamespace::func6();
}

void func1()
{
    MyFunc2();
    func3();
}

int main()
{
    MyFunc2();
    func1();
    return 0;
}
