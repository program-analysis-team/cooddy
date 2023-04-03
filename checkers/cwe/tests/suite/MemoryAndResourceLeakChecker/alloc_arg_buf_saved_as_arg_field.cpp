#include <stdlib.h>

#include <cstdint>

void allocArgBuff(uint32_t pid, uint32_t partitionID, void** outstructurePtr, const char* funcName,
                    const int32_t fileLine)
{
    *outstructurePtr = malloc(0x1234);
}

struct A {
    char* ptr;
};

struct B {
    A* a;
};

A* getSomeA();

void insertNode(char** in, char** out)
{
    *in = *out;
}

int initFunc(B* val)
{
    A* data = val->a;
    char* ptr;
    allocArgBuff(123, 123, ((void**)&ptr), "_function_name_", 4586);
    insertNode(&data->ptr, &ptr);

    return 0;
}
