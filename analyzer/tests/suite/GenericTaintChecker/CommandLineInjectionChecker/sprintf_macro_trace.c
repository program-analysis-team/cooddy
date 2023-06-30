#include <stdio.h>
#include <stdlib.h>

#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif

typedef struct str {
    int num;
} customStruct;

#define custom_snprintf(strDest, destMax, format, ...) snprintf((strDest), (destMax), (format), ##__VA_ARGS__)

void EXTER_ATTACK bar(const int* num, customStruct* strct);

void foo(int* num, char* buf)
{
    customStruct localStruct;
    bar(num, &localStruct);
    custom_snprintf(buf, 64, "kill -9 %d", localStruct.num);
    system(buf);
}
