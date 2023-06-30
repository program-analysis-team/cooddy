#include "new_delete_macro.h"

#include <iostream>

class A {
public:
    A(int val) : a(val) {}

    int multiple2times()
    {
        a *= 2;
        return a;
    }

private:
    int a;
};

A* createObj();
void freeObj(A* ptr);

int main()
{
    A* a = NULL;

    int b = 0;
    std::cin >> b;

    if (b < 10) {
        A* a = createObj();

        std::cout << a->multiple2times() << std::endl;
    }
    freeObj(a);
    return 0;
}

A* createObj()
{
    return ALLOC_MEM A(6);
}

void freeObj(A* ptr)
{
    FREE_MEM(ptr)
}
