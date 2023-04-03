#include "header.h"

extern int *get_ptr();

#ifndef FOO_DEF
int main_foo()
{
    int *p = get_ptr();
    return *p;
}
#endif
