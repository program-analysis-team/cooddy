
#include "string.h"

int main()
{
    char sPtr[6] = "Hello";
    char dPtr[sizeof (sPtr)];
    strcpy(dPtr, sPtr);  // no error
    return 0;
}
