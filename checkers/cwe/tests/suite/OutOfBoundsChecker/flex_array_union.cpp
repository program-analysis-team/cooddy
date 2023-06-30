#include <cstdint>
#include <cstdlib>

union AAA1 {
    uint32_t a;
    uint8_t b[1];
};

void foo1()
{
    AAA1 a;
    a.a = 0xFFFFFFFF;
    *(uint32_t*)a.b = 42;  // reading of 32 bit is OK
    a.b[4] = 0;            // OOB
}

union AAA2 {
    uint8_t b[1];
    uint32_t a;
};

void foo2()
{
    AAA2 a;
    a.a = 0xFFFFFFFF;
    *(uint32_t*)a.b = 42;  // reading of 32 bit is OK
    a.b[4] = 0;            // OOB
}

void foo3()
{
    AAA2* a = (AAA2*)malloc(sizeof(AAA2) + sizeof(AAA2::b[0]) * 5);
    a->b[8U];  // OK
    a->b[9U];  // OOB
}
