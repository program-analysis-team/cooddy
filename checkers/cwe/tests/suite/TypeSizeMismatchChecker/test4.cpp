#include <stdlib.h>

struct A {
    int x;
    int y;
};

struct B {
    int x;
    int y;
    int z;
};

void init(void* ptr){
    struct B *b = (struct B*) ptr;
    b->x = 0;
    b->y = 0;
    b->z = 0; // Sink
}

int main(int argc, char* argv[]){
    struct A * aptr = (struct A*) malloc(sizeof(struct A));
    init((void*) aptr);
    free(aptr);
    return 0;
}
