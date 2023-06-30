void* memcpy(void* destination, const void* source, unsigned num);

#define MEMCPY(d, s, n) (memcpy(d, s, n))
#define MY_MYMEMCPY(d, s, n) (MEMCPY(d, s, n))

struct A {
    void* dst;
};

void foo(A* a, void* src)
{
    a->dst = nullptr;
    MY_MYMEMCPY(a->dst, src, 42);
}
