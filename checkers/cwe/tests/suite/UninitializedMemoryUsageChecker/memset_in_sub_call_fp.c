#include <string.h>

typedef struct _A {
    int a;
} A;

int some_call();

void use(A *a)
{
    a->a;  // no error here
}

int sub_init(A *a)
{
    memset(a, 0, sizeof(A));
    if (some_call() != 0) {
        return 1;
    }
    return 0;
}

int init(A *a, char *name)
{
    if (sub_init(a) != 0) {
        return 1;
    }
    use(a);
    return 0;
}

void foo(char *name)
{
    A a;
    init(&a, name);
}
