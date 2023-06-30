#include <memory.h>

void func1();

template<int>
struct MyClass {
    static void func5()
    {
        memset(0, 0, 10);
    }
};

namespace MyNamespace {
void func6()
{
    MyClass<5>().func5();
}
};  // namespace MyNamespace
