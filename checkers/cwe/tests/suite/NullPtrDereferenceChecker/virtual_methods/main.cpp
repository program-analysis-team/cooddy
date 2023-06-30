#include "derived2.h"

void bar()
{
    Derived3 d3;
    foo(&d3, nullptr);  // no error

    Derived2 d2;
    foo(&d2, nullptr);  // error here
}

int g_var[10];  // for ast dump test
