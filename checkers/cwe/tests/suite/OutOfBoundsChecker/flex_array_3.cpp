#include <cstdint>
#include <cstdlib>

struct AAA1 {
    uint16_t sz;
    uint16_t buff[1];
};

void bar(AAA1* a)
{
    a->buff[6U];  // OOB
}

void foo1()
{
    AAA1* a = (AAA1*)malloc(sizeof(AAA1) + sizeof(AAA1::buff[0]) * 5);
    bar(a);
    free(a);
}
