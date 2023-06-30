#include <stdio.h>
#include <stdlib.h>
struct S {
    void* data;
};
void foo() {
    struct S s;
    free(s.data); // FP here, variable is on stack
}
