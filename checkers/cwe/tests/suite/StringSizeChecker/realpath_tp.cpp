#include <limits.h>

char* realpath(const char* restrict_path, char* restrict_resolved_path);

void test_bad_1()
{
    char path1[259] = "", path2[PATH_MAX];
    realpath("test.txt", path2);  // no error
    realpath("test.txt", path1);  // error
}
