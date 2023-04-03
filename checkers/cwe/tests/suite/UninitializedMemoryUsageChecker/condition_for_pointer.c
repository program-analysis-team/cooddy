#include <stdint.h>

int func(uint16_t* p, int param)
{
    uint16_t v = 10;
    if (param == 1)
        return -1;
    if (p)
        *p = v;
    return 1;
}

int boo(int param)
{
    uint16_t p;
    func(0, param);
    int rc = func(&p, param);
    if (rc < 1)
        return 0;
    return p;
}
