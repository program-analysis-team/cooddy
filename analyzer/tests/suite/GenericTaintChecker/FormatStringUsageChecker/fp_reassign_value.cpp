#include <stdio.h>

#include <fstream>
#include <string>

int main() {
    char buff[100];
    FILE *f = fopen("test.txt", "r");
    fread(buff, 1, sizeof(buff), f);
    char *fmt = buff;
    fmt = "%d";
    printf(fmt, 10);
    return 0;
}
