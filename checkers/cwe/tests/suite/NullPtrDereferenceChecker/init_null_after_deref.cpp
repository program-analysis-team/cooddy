#include <stdlib.h>

int main()
{
    int* p = (int*)malloc(123);
    int b = p != nullptr ? *p : 123;
    p = nullptr;
}
