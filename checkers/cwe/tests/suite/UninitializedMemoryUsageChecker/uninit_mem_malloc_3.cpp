#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct A {
    int b;
    char * a;
};

void foo(int* arg){
    *arg = 123;
}

int main(int argc, char* argv[]){
    struct A *a = (struct A*)malloc(sizeof(struct A));
    if(!a) {
        return -1;
    }

    if(argv[1][0] == 'a'){
        a->a = (char*)malloc(sizeof(char)*9);
        strcpy(a->a, "0x000001");
        printf("%s %d\n", a->a, a->a);
    } else if(argv[1][0] == 'b'){
        a->a = (char*) malloc(sizeof(char)*9);
        strcpy(a->a, "0x000002");
        printf("%s %d\n", a->a, a->a);
    } else {
        printf("No allocation of a->a\n");
        free(a->a); // FN: uninit usage here
        free(a);
    }

    return 0;
}
