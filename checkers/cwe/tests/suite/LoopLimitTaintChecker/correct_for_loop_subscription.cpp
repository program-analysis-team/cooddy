#include <cstdlib>
#include <fstream>
#include <iostream>
#include <cstdint>

struct A {
    char* str;
};
void EXTER_ATTACK getA(A* a);
void iterateStr(const char* str)
{
    uint32_t i;
    for (i = 1; str[i] != '\0'; i++) {
    }
}
void f1()
{
    A a;
    getA(&a);
    iterateStr(a.str);
}
