#include "header.h"

extern int* get_ptr();

int bar1()
{
    int* p = get_ptr();
    return *p;  // no error
}
