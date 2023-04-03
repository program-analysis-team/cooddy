#include <cstdint>
#define SZ 6
struct Boo1 {
    uint8_t buff[SZ];
};

struct Boo2 {
    void* buff;
    int len;
};

void baz2(void* buff, int len)
{
    uint64_t x = *(uint64_t*)buff & ~0xFFULL;
}

void foo(Boo2* boo)
{
    baz2(boo->buff, boo->len);
}

void bar(Boo1* b)
{
    Boo2 boo = {0};
    boo.buff = b->buff;
    boo.len = SZ;
    foo(&boo);
}
