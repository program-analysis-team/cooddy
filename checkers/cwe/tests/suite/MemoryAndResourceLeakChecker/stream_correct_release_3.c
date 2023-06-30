#include <stdio.h>

void test_1(char* fName, int x)
{
    FILE* fp;
    if ((fp = fopen(fName, "w")) == NULL) {
        return;
    }
    int fd = fileno(fp);
    close(fd);
}

void test_2(char* fName)
{
    int fd = open(fName, 0);
    if (fd < 0) {
        return;
    }
    FILE* fp = fdopen(fd, "r");
    fclose(fp);
}
