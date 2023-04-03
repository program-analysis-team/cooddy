#include <string.h>

int scanf_s(const char* format, ...);

void cwe120_c_186(char* buf, size_t bufSize, char* msg)
{
    size_t len = strlen(msg);
    scanf_s("%s", buf, len);
}

int main(int argc, char** argv)
{
    char buf[16];
    if (argc > 1) {
        cwe120_c_186(buf, sizeof(buf), argv[1]);
    }
    return 0;
}
