#include <stdlib.h>

int* func(bool flag)
{
    static int ret = 1;
    return flag ? 0 : &ret;
}

int main()
{
    int* ret1 = func(false);
    int* ret2 = func(true);
    int a = *ret1;  // no error here
    return *ret2;   // error here
}
