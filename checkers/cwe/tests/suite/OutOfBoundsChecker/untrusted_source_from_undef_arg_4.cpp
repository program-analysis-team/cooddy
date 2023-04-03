#define SZ 0x42
struct Bar {
    int arr[SZ];
    int sz;
};

#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif // EXTER_ATTACK

void EXTER_ATTACK foo1(Bar* bar)
{
    bar->arr[bar->sz];  // error here
}

void EXTER_ATTACK foo2(Bar* bar)
{
    if (bar != nullptr && bar->sz >= 0 && bar->sz < SZ) {
        bar->arr[bar->sz];  // no error here
    }
}
