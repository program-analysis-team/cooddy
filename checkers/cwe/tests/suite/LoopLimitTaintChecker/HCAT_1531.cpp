#include <stdio.h>
#include <stdlib.h>

char* to_upper(char* str, int len){
    for (int i = 0; i < len; i++) {
        if (*(str + i) >= 'a' && *(str + i) <= 'z') {
            *(str + i) = *(str + i) - ((int)'a' - (int)'A');
        } else {
            *(str + i) = ' ';
        }
    }
    return str;
}

int EXTER_ATTACK main(int argc, char* argv[])
{
    if (argc < 3) {
        return -1;
    }

    printf("%s\n", to_upper(argv[1], std::atoi(argv[2])));
    return 0;
}
