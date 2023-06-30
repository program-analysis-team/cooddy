#include <cstdlib>

int foo1()
{
    static int* ptr = 0;
    if (ptr == nullptr) {
        ptr = (int*)malloc(sizeof(int));
    }
    return *ptr;
}

int foo2()
{
    static int* ptr = (int*)malloc(sizeof(int));
    return *ptr;
}
