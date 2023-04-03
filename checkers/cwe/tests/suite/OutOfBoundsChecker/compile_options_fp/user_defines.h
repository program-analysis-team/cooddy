#include <stdint.h>

typedef struct {
    uintptr_t p;
} S;

typedef struct {
    S v[100];
} Foo;
