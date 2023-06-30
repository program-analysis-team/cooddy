#include <cstring>
#include <cstdio>

int test(FILE * openedfile) {
    char ch;
    int rv; //return value from fscanf
    while(1)
    {
        rv = fscanf(openedfile,"%c",&ch);
        if (rv == EOF) return rv;
        if (ch == ':')
            break;
    }
}
