#include <stdlib.h>

#include "header.h"

int* get_ptr()
{
    MyStruct a;
    int b = a.fieldTwo;

    return (int*)0xdeadbeef;
}
