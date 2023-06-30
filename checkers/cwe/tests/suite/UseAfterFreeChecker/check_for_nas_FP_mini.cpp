
#include <stdlib.h>
#include <iostream>
struct SomeStruct {
};
int initialize(SomeStruct*& pSomeStruct, int a) {
    if (pSomeStruct != NULL) {
        delete pSomeStruct;
    }
    pSomeStruct = new SomeStruct;
    if (a > 0) {
        delete pSomeStruct;
        return 2;
    }
    return 0;
}
void test(SomeStruct* pSomeStruct)
{
    if (!initialize(pSomeStruct, 0)) {
        *pSomeStruct;
    }
}
