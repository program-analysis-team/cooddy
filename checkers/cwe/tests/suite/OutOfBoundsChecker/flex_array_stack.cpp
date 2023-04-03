#include <cstdint>
#include <cstdlib>

struct BBB1 {
    uint32_t a;
    uint32_t b[1];
};

void boo1()
{
    BBB1 b;
    b.b[0];  // OK
    b.b[1];  // OOB
}

struct BBB2 {
    uint32_t a;
    uint16_t b[1];
};

void boo2()
{
    BBB2 b;
    b.b[0];  // OK
    b.b[1];  // also ok because of 32-bit alignment
    b.b[2];  // OOB
}
