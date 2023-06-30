#include <cstdint>
#include <cstdlib>

struct AAA1 {
    uint16_t sz;
    uint16_t buff[1];
};

struct AAA2 {
    uint16_t sz;
    AAA1 buff[2];
};

void bar(AAA2* a2)
{
    a2->buff[5];    // no report here
}
