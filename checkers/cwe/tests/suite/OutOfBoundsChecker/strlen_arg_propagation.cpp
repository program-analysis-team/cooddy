#include <stdlib.h>

#include <cstring>

void boo(const char* str)
{
    int arr[10] = {};
    int idx = strlen(str);
    arr[idx];  // no error here
}

#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif // EXTER_ATTACK

void EXTER_ATTACK foo(int x, const char* str)
{
    int arr[10] = {};
    int idx = strlen(str);
    arr[idx];  // error here due to EXTER_ATTACK
}
