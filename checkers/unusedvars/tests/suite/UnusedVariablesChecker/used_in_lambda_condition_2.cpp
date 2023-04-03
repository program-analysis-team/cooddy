#include <functional>

void Foo(int& var)
{
    std::function<void(int&)> addAddrOfConditions = [&](int& it) {
        int numArgs = it + 3;
        if (numArgs != 0) {
            it = numArgs;
        }
    };
    addAddrOfConditions(var);
}
