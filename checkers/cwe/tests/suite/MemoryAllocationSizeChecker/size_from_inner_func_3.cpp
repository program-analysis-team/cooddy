#include <cstdlib>

struct AAA {
    int x;
    int y;
};

void bar1(const char* str, AAA* a)
{
    a->x = atoi(str);
}

void EXTER_ATTACK taint(char*);

void foo()
{
    AAA a;
    char* str;
    taint(str);
    bar1(str, &a);
    malloc(a.x);
    malloc(a.y);
}
