#include <stdio.h>

int main() {
    char input[200];
    scanf("%s", input);

    const char *format;
    format = input;
    printf(format, 10);
    return 0;
}
