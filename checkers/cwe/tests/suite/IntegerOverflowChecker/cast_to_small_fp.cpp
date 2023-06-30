#include <stdio.h>

struct S {
    int v;
    char operator[](int idx)
    {
        return 0;
    }
};

void boo()
{
    S s;
    for (size_t i = 0; i < 10; i++) {
        s[i];  // FP here
    }
}
void bar(short v);
void foo()
{
    size_t i = 0;
    int j = (int)i;  // FP here - C-style cast
    S s{int(i)}; // FP here
    static constexpr unsigned int MAX_ALLOC_SIZE_BITS = 31;
    bar(MAX_ALLOC_SIZE_BITS); // FP here - value is constant
}
