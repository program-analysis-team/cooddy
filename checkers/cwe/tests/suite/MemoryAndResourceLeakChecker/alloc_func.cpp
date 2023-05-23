#include <stdlib.h>

char* myAlloc(int bytes) {
    return (char*)malloc(bytes);
}

void myFree(char* buffer) {
    free((void*)buffer);
}

void myAllocFree(int state) {
    char *pBuffer = myAlloc(50);
    if(pBuffer == 0) {
        return;
    }
    if(state == 0) {
        return;
    }
    myFree(pBuffer);
}

void test_25() {
    myAllocFree(1);
}