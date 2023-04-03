#include <cstdint>

int scanf_s(const char* _Format, ...);

int f(char* s, int size)
{
    scanf_s("%s", s, size);
}
