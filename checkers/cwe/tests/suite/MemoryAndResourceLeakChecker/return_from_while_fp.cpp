#include <stdio.h>

static unsigned int get_file_size(const char* log_path, unsigned int max_file_size)
{
    FILE* fp = fopen(log_path, "r");
    if (fp == 0) {
        return 0;
    }
    unsigned int len = 0;
    while (len < max_file_size && log_path[len] == '\n') {
        len++;
    }
    fclose(fp);
    return len;
}
