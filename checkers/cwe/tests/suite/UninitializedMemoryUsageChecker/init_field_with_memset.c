#include "string.h"

struct A {
    int a;
    int b;
};

int bar(struct A *a, int x) {
    if (x) {
        memset(a, 0, sizeof(struct A));
        return 1;
    }
    return 0;
}

int foo(int x) {
    struct A a;
    if (bar(&a, x)) {
        return a.a;
    }
    return 0;
}
