#include <stdlib.h>
#include <stdio.h>

void foo(int * a, int b){
    *a = 5;
}

void bar(){
    int *c = (int*) std::malloc(sizeof(int));
    free(c);
    foo(c, 5);
}
