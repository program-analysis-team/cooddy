#include <stdlib.h>

#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif  //  EXTER_ATTACK

void EXTER_ATTACK foo(int c, const char* s1, const char* s2)
{
    int arr[10] = {0};
    const char* a = s1;
    if (c == 10)
        return;

    if (c == 10) {
        a = s1;
    }
    if (c == 20) {
        a = s2;
    }
    auto idx = atoi(a);
    arr[idx];  // error here
}
