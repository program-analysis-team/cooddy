#include <stdlib.h>

void func()
{
    exit(1);
}

int main()
{
    int *p = 0;
    if (p == 0)
        func();
    return *p;
}
