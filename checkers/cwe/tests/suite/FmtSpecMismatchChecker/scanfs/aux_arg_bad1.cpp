#include <cstdint>

int scanf_s(const char* _Format, ...);

int f(char* s, int64_t size)
{
    scanf_s("%s", s, size);
}
