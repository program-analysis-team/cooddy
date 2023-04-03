#include <string.h>
int memcpy_s(void* dest, size_t destsz, void* src, size_t count);

void boo(char* p, int l)
{
    char buff[] = "Unknown\0";
    memcpy_s(p, l, buff, strlen(buff) + 1);
}
