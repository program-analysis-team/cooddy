#include "func.h"

#include <iostream>

const char *GetString()
{
    std::cout << __INTRO_STRING__ << std::endl;
    return __MY_STRING__;
}
