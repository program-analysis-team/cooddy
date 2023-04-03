#ifndef EXTER_ATTACK
#define EXTER_ATTACK __attribute__((annotate("__cooddy_security_risk")))
#endif // EXTER_ATTACK

typedef unsigned uint32_t;

void bar(uint32_t idx)
{
    char arr[10] = {0};
    arr[idx];  // error here
}

void EXTER_ATTACK foo(uint32_t size)
{
    uint32_t i = 0;
    while (i < size) {
        bar(i++);
    }
}
