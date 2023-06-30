#include <iostream>
#include <stdlib.h>

int foo(int n, int* zLine) {
    while(n > -5){
        if (n == 0) {
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
        return 2;
    }
}

#define isFive(a) (a == 5)


int main(){
    int* zLine = (int*)malloc(100);
    int q = 3;
    while(q >= 0){
        q = bar(zLine, q);
        if (q == 2){
            zLine[0] = 123;
        }
        if (zLine[0]>3 && isFive(zLine[0])){
            q = 7;
        }
    }
    free(zLine);
    return 0;
}
