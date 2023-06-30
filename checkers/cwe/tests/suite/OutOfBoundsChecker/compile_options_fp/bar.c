#include "user_defines.h"

int _checkFoo(Foo* f);

void bar(Foo* f, uint32_t v)
{
    uint32_t idx = v >= 100 ? 99 : v;
    if (idx >= 100) {
        return;
    }
    _checkFoo(f);
    f->v[idx].p = 0;
}
