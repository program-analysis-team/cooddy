#include <iostream>

#define UNRELATED_ANNOTATION __attribute__((annotate("unrelated_annotation")))

#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif // EXTER_ATTACK

static int UNRELATED_ANNOTATION EXTER_ATTACK f(int idx, char x)
{
    int arr[10] = {};

    return arr[idx];
}
