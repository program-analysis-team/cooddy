#include <iostream>
#include <stdlib.h>

int foo(int n, int* zLine) {
    while(n > -5){
        if (n == 0) {
            free(zLine);
            return 0;
        }
        zLine[n] = 0;
        n--;
    }
    return zLine[1];
}

int bar(int* zPrior, int a){
    if (a < 0) {
        return foo(a, zPrior);
    } else {
        free(zPrior);
        return 3;
    }
}

bool isFive(int a){
    return a == 5;
}

int main(){
    int* zLine = (int*)malloc(100);
    int q = 3;
    while(q >= 0){
        q = bar(zLine, q);
        if (q == 3) {
            zLine[0] = 123;
        }
    }
    free(zLine);
    return 0;
}
