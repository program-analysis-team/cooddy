#include <stdlib.h>

#include <cstdint>
extern int32_t allocBuf(uint32_t structSize, void** outStructPtr, const char* funcName, const int32_t fileLine, uint32_t owner_modid){
    *outStructPtr = malloc(0x1234);
    return 0;
}

extern void freeBuf(void * objPtr, uint32_t pid, const char* funcName, const int32_t fileLine){
    free(objPtr);
}

struct A {
    int a;
};

int undefFunc(char* val);

int test(void* structPar)
{
    uint32_t valueLen = 123;
    void* valueBuf = structPar;
    int32_t ret = 0;
    bool isRetry = false;
    for (;;) {
        ret = undefFunc((char*)valueBuf);
        if (ret == 0) {
            break;
        }
        ret = allocBuf((valueLen), (&valueBuf), "_function_name_", 419, 987);
        if (ret != 0 || valueBuf == nullptr) {
            ret = 456;
            break;
        }
        isRetry = true;
    }
    if (isRetry && valueBuf != nullptr) {
        freeBuf((valueBuf), 987, "_function_name_", 445);
    }
    return ret;
}

int main()
{
    A a = {0};
    test((void*)&a);
}
