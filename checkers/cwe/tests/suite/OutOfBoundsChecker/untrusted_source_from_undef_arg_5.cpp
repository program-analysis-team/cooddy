#define SZ 10
struct Boo1 {
    int buff[SZ];
};

struct Boo2 {
    int* buff;
    int len;
};

void baz1(int* buff, int len, unsigned index)
{
    if (index < SZ) {
        int x = buff[index];
    }
}

void baz2(int* buff, int len, unsigned index)
{
    int x = buff[index];
}

#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif // EXTER_ATTACK

unsigned EXTER_ATTACK get_index();

void foo(Boo2* boo)
{
    auto index = get_index();
    baz1(boo->buff, boo->len, index);
    baz2(boo->buff, boo->len, index);
}

void bar(Boo1* b)
{
    Boo2 boo = {0};
    boo.buff = b->buff;
    boo.len = SZ;
    foo(&boo);
}
