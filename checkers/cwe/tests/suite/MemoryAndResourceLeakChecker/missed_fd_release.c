#include <fcntl.h>
#include <stdio.h>

void test_open(const char* fName, int f)
{
    int fd = open(fName, O_RDONLY);
    if (fd < 0) {
        return;
    }
    if (f) {
        return;
    }
    close(fd);
}

void test_open64(const char* fName, int f)
{
    int fd = open64(fName, O_RDONLY);
    if (fd < 0) {
        return;
    }
    if (f) {
        return;
    }
    close(fd);
}
