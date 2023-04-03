#include "header.h"

extern int* get_ptr();

int bar()
{
    int* p = get_ptr();
    return *p;  // no error
}
