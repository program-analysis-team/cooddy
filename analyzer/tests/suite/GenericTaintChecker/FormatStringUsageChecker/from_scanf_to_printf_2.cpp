#include <stdio.h>

int main() {
    char input[200];
    scanf("%s", input);

    if (input[0] == 'a') {
        const char *format = input;
        printf(format, 10);
    }
    return 0;
}
