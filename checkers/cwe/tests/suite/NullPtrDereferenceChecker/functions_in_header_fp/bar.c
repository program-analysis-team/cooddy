#include "header.h"

int bar()
{
    int* p = 0;
    get_ptr(&p);
    return *p;  // no error
}
