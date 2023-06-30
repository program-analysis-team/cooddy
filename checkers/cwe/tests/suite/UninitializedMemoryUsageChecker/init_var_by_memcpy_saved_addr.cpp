#include <string.h>

int main()
{
    double v;
    double r = 0;
    auto addr_dst = &v;
    auto addr_src = &r;
    memcpy(addr_dst, addr_src, sizeof(r));
    double c = v + 1;
    return 0;
}
