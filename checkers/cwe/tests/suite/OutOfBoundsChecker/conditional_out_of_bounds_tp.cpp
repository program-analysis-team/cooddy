#include <stdlib.h>

#include <cstring>

#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif //  EXTER_ATTACK

int someCondition();
void EXTER_ATTACK foo(int c, int x)
{
    int a = 7;
    int b;
    int arr[10] = {0};
    if (c == 10) {
        a = x;
    }

    memcpy(&b, &a, sizeof(int));
    arr[b];  // error here
}
