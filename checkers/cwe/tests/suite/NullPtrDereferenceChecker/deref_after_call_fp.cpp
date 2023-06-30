#include <iostream>

int* func()
{
    return new int[10];
}

int main()
{
    int* a = nullptr;
    if (a == nullptr)
        a = func();
    std::cout << a;
    return *a;
}
