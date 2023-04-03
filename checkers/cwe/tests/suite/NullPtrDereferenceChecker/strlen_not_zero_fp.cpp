#include <string.h>

void foo(int** ptr, size_t size)
{
    if (size == 0) {
        *ptr = nullptr;
    }
}

void bar(int* ptr, const char* str)
{
    foo(&ptr, strlen(str) + 1);
    int x = *ptr;  // FP here
    delete ptr;
}
