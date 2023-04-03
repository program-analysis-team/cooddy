#include <cstdio>

void foo(void* buffer, size_t size, size_t count, FILE* stream)
{
    size += 1;

    fread(buffer, size, count, stream);
}
