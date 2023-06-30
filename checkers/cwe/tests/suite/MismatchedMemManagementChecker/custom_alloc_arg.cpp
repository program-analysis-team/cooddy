#include <cstdint>
#include <stdlib.h>
extern int32_t allocBuf(uint32_t structSize, void** outStructPtr, const char* funcName, const int32_t fileLine, uint32_t owner_modid){
    *outStructPtr = malloc(0x1234);
    return 1;
}

extern void freeBuf(void * objPtr, uint32_t pid, const char* funcName, const int32_t fileLine){
    free(objPtr);
}

struct A {
    int a;
};
int test(void* structPar) {
    uint32_t valueLen = 123;
    void *valueBuf = structPar;
    allocBuf((valueLen), (&valueBuf), "_function_name_", 419, 987);
    freeBuf((valueBuf), 987, "_function_name_", 445);
}

int main() {
    A a = {0};
    test((void*) &a);
}
