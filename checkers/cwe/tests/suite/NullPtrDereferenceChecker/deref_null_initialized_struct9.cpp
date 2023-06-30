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
    memset(*a, 0, sizeof(A));
    (*a)->pointer = new int[10];
}
int main()
{
    A* a = NULL;
    create(&a);
    return *(*a).pointer;
}
