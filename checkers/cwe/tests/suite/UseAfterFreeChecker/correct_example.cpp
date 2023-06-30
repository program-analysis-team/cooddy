#include <stdio.h>

int main() {
    char input[200] = "Hello, world!";

    const char *format;
    format = input;
    printf(format, 5);
    return 0;
}
