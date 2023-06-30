#include <stdlib.h>

#include <cstring>

#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif // EXTER_ATTACK

void EXTER_ATTACK foo(int c, int x)
{
    int a = 7;
    int b;
    int arr[10] = {0};
    if (c == 10) {
        a = x;
    }
    auto alias = &b;
    memcpy(alias, &a, sizeof(int));
    arr[b];  // error here
}
