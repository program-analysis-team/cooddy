#include <stdlib.h>

void SomeFunc(unsigned int *para);

void Foo()
{
    unsigned int len = 8;
    SomeFunc(&len);  // Untrusted data source
    int valueLen = len;
    unsigned int uvalueLen = len;
    char *valueBuf = malloc(len);
    if (valueBuf != NULL && len > 0) {
        valueBuf[uvalueLen - 1] = 0;  // No OOB FP here
        valueBuf[valueLen - 1] = 0;   // OOB here
    }
    free(valueBuf);
}
