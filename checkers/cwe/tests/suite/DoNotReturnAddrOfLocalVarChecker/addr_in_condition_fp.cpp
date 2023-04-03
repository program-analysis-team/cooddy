#include <memory.h>

int foo()
{
    int res, v = 0;
    if (0 != &res)
        memcpy(&res, &v, sizeof(int));
    return res;
}
