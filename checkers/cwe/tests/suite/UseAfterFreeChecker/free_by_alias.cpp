#include <malloc.h>

void MemUafWithAlias()
{
    char *p;
    char *alias;

    p = (char *)malloc(100);
    if (p == nullptr)
        return;
    alias = p;
    free(p);
    *alias = 5;  // Uaf
}

void MemUafTest()
{
    MemUafWithAlias();
}
