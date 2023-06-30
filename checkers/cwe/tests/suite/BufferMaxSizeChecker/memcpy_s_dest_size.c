#include <string.h>
#define SRC_STR "0123456789abcde0123"

typedef struct _charVoid {
    char charFirst[16];
    void * voidSecond;
    void * voidThird;
} charVoid;

void TestBadCase01()
{
    charVoid structCharVoid;
    structCharVoid.voidSecond = (void *)SRC_STR;
    memcpy_s(structCharVoid.charFirst, sizeof(structCharVoid), SRC_STR, sizeof(structCharVoid));
    structCharVoid.charFirst[(sizeof(structCharVoid.charFirst) / sizeof(char)) - 1] = '\0'; /* null terminate the string */
}
