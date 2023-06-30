#include <stdlib.h>

int foo(bool flag){
    int *ptr;
    ptr = (int*)malloc(sizeof(int));
    if (flag) {
        delete ptr;
    }
    free(ptr);
}

int boo(int flag) {
    foo(flag);
}
