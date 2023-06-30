#include <cstdint>
#include <cstdlib>

struct AAA1 {
    uint16_t sz;
    uint16_t buff[1];
};

void foo1()
{
    AAA1* a = (AAA1*)malloc(sizeof(AAA1) + sizeof(AAA1::buff[0]) * 5);
    a->buff[5U];  // OK
    a->buff[6U];  // OOB
    free(a);
}

struct AAA2 {
    uint32_t sz;
    uint16_t buff[1];
};

void foo2()
{
    AAA2* a = (AAA2*)malloc(sizeof(AAA2));
    a->buff[0];  // OK
    a->buff[1];  // also OK because of 32-bit alignment
    a->buff[2];  // OOB
    free(a);
}
