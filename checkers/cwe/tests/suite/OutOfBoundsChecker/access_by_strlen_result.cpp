#include <string.h>

#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif // EXTER_ATTACK

int EXTER_ATTACK foo(const char* name)
{
    int arr[20];
    int len = strlen(name);
    return arr[len];  // error here
}
