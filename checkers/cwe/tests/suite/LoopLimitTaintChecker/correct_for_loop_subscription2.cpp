#include <cstdlib>
#include <fstream>
#include <iostream>
#include <cstdint>

struct A {
    char* str;
};
void EXTER_ATTACK getA(A* a);

void f1()
{
    A a;
    getA(&a);
    uint32_t i;
    for (i = 1; a.str[i] != '\0'; i++) {
    }
}
