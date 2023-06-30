#define SZ 10
#define MIN(a, b) ((a) < (b) ? (a) : (b))

struct A {
    int arr[SZ];
    unsigned cnt;
};

void foo1(A* a)
{
    if (!a) {
        return;
    }
    unsigned j = MIN(SZ, a->cnt);
    for (unsigned i = 0; i < j; j++) {
        a->arr[j];
    }
}

void foo2(A* a)
{
    if (!a) {
        return;
    }
    unsigned j = MIN(a->cnt, SZ);
    for (unsigned i = 0; i < j; j++) {
        a->arr[j];
    }
}
