#include <string.h>
typedef struct A {
    int* pointer;
} A;
void bar(int* p)
{
    int b = *p;
    int c = p[5];
}
void updateInfo(A* a)
{
    a->pointer = new int[10];
    memset(a->pointer, 0, sizeof(int) * 10);
}
void foo()
{
    A a;
    updateInfo(&a);
    bar(a.pointer);
}
