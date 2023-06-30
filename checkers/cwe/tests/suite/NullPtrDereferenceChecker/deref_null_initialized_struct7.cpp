#include <string.h>
typedef struct tarPlogUtConfig {
    char param[10];
} PlogUtConfig;

PlogUtConfig g_plogUtConfig;

void foo(char* argv[])
{
    char* c;
    memset(&g_plogUtConfig, 0, 10);
    strncpy(g_plogUtConfig.param, c, 1);
}
