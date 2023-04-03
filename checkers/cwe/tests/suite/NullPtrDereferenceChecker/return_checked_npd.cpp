struct A{
    char* b;
};

#define unlikely(x)     __builtin_expect((x), 0)
char* test(int val)
{
    A *compMem = nullptr;
    if (unlikely(compMem == nullptr)) {
        return (char*)1234;
    }
    return compMem->b;
}
