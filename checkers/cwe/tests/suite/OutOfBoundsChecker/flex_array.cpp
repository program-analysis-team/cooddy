#include <cstdint>
#include <cstdlib>

struct AAA1 {
    uint16_t sz;
    uint16_t buff[1];
};

struct AAA2 {
    uint16_t sz;
    uint16_t buff[2];
};

struct AAA3 {
    uint16_t sz;
    uint16_t buff[33];
};

struct AAA4 {
    uint16_t sz;
    uint16_t buff[2];
    uint16_t x;
};

void bar(AAA1* a1, AAA2* a2, AAA3* a3, AAA4* a4)
{
    a1->buff[5];    // no report here
    a2->buff[5];    // no report here
    a3->buff[333];  // OOB
    a4->buff[5];    // OOB
}
