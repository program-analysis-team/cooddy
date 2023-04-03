#include <stdlib.h>

int main()
{
    int* p = 0;
    int var = 10;
    if (1)
        p = &var;
    return *p;  // FP no error here
}
