#include "header.h"

int foo()
{
    int* p = 0;
    get_ptr(&p);
    return *p;  // no error
}
