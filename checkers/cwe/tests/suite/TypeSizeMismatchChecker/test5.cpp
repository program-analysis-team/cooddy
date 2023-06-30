#include <stdlib.h>

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
    int *intarr = (int*) malloc(sizeof(int)*2);
    init((void*) intarr);
    free(intarr);
    return 0;
}
