#include <string.h>

char g_buff[100] = {0};

typedef void (*tfunc)(void *p, int *rc);
tfunc g_func;

typedef struct _A {
    char **pbuff;
} A;

void get_buff_async(char **buff, int *rc)
{
    A a;
    a.pbuff = buff;
    g_func(&a, rc);
}

void get_buff(int v, char **buff, int *rc)
{
    if (v == 10) {
        *buff = g_buff;
        *rc = 0;
        return;
    }
    get_buff_async(buff, rc);
}

int bar(int v, char *buff)
{
    int rc = 0;
    get_buff(v, &buff, &rc);
    if (rc != 0) {
        return rc;
    }
    memset(buff, 0, 10);  // no error here
    return 0;
}

int foo(int v)
{
    char *ptr = 0;
    bar(v, ptr);
}
