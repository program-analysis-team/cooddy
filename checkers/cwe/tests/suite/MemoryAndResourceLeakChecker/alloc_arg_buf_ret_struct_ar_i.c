#include <stdlib.h>

#include <stdint.h>

void allocArgBuff(uint32_t pid, uint32_t partitionID, void** outstructurePtr, const char* funcName,
                    const int32_t fileLine)
{
    *outstructurePtr = malloc(0x1234);
}

struct A {
    char* ptr[5];
};

struct A* getSomeA();

struct A* initFunc()
{
    struct A* data = (struct A*)malloc(0x1234);
    int i =3;
    allocArgBuff(123, 123, ((void**)&(data->ptr[i])), "_function_name_", 4586);

    return data;
}
