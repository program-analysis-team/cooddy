#include <stdlib.h>

int* ret_null()
{
    return nullptr;
}

int main()
{
    int* p = ret_null();
    if (p == 0)
        return -1;
    int b = *p;
}
