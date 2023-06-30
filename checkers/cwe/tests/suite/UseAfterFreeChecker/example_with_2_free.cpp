#include <stdlib.h>

int main(int argc, char* argv[]){
    int *a = (int*) malloc(sizeof(int));
    int *b = (int*) malloc(sizeof(int));

    *b = 1;
    *a = 0;
    free(b);
    *a = -1;
    free(a);
    *b = 0; //nullptr;
    *a = 0; //nullptr;
}
