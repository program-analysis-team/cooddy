#include <iostream>

#define UNRELATED_ANNOTATION __attribute__((annotate("unrelated_annotation")))

static int UNRELATED_ANNOTATION /* EXTER_ATTACK */ f(int idx, char x)
{
    int arr[10] = {};

    return arr[idx];
}
