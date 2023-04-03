#include "user_defines.h"

// Two functions as a workaround for different mangling rules for 32/64-bit compilation.
// See https://stackoverflow.com/a/13386056

int checkFoo(Foo* f)
{
    return f->v[0].p == 0;
}

int _checkFoo(Foo* f)
{
    return f->v[0].p == 0;
}
