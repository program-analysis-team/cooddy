#include <cstdlib>
#include <cstring>

void SomeFunc(int *para);

int main()
{
    int len = 8;
    SomeFunc(&len);  // Untrusted data source
    int valueLen = len;
    unsigned int uvalueLen = len;
    int *valueBuf = (int *)malloc(valueLen);
    if (valueBuf != nullptr) {
        memset(valueBuf, 0, uvalueLen);  // No OOB FP here
        memset(valueBuf, 0, valueLen);   // No OOB here
    }
    return 0;
}
