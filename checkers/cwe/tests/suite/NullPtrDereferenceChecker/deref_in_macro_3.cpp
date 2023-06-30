void* memcpy(void* destination, const void* source, unsigned num);

#define MEMCPY(d, s, n) (memcpy(d, s, n))
#define MY_MYMEMCPY(d, s, n) (MEMCPY(d, s, n))

void foo(void* src)
{
    void* dst = nullptr;
    MY_MYMEMCPY(dst, src, 42);
}
