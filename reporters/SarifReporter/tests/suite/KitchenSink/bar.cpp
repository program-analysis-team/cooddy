#include "header.h"

int* f(bool condition)
{
    if (condition) {
        return new int(1);
    }

    return nullptr;
}
