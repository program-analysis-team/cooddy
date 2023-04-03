#include <stdlib.h>

int main()
{
    const char* p = 0;
    if (p == 0)
        p = "Hello";
    return *p;
}
