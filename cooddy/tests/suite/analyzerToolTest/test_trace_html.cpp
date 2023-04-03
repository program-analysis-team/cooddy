#include <vector>
template<class T>
struct Alloc { };
template<class T>
using Vec = std::vector<T, Alloc<T>>;

int * Boo() {
    return nullptr;
}

int EXTER_ATTACK UndefFunc();

int UndefFunc2();

int foo()
{
    auto v = Boo();
    int a[10];
    return *v  + a[UndefFunc()] + a[UndefFunc2()];
}
