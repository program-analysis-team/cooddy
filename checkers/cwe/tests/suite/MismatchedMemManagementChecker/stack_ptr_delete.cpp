#include <stdlib.h>
void boo(int* ptr) {
    delete ptr;
}

void foo()
{
    int ptr[5];
    boo(ptr);
}
