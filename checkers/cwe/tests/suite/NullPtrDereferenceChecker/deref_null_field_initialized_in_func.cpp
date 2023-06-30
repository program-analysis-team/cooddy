#include <string.h>
typedef struct A {
    int* pointer;
} A;
int* foo();
void create(A** a)
{
    *a = new A;
    if (*a == NULL)
        return;
    (*a)->pointer = 0;
}
int main()
{
    A* a = NULL;
    create(&a);
    return *(*a).pointer;
}
