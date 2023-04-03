#include <malloc.h>
#include <string.h>

void foo(const char* name)
{
    char* mem = (char*)malloc(strlen(name) + 1);
    mem[0] = 0;  // no problem here
    free(mem);
}
