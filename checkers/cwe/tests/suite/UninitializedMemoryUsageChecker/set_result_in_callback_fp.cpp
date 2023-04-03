#include <string.h>

typedef void (*tfunc)(void *arg);

void get_data_ex(int v, void *data, tfunc callback, void *arg);

typedef struct _A {
    int result;
} A;

void get_data_cb(void *arg)
{
    ((A *)arg)->result = 1;
}

void get_data(int v, void *data, tfunc callback, void *arg)
{
    if (v == 1) {
        if (callback != 0) {
            callback(arg);
        }
        return;
    }
    get_data_ex(v, data, callback, arg);
}

int get_data_sync(int v, void *data)
{
    int retry = 3;
    A a;
    do {
        get_data(v, data, get_data_cb, &a);
        if (a.result != 1) {
            break;
        }
    } while (--retry);

    if (a.result != 0) {
        return a.result;
    }
    return 0;
}

int foo(int v)
{
    int b;
    int rc = get_data_sync(v, &b);
    if (rc == 0) {
        return b;
    }
    return 0;
}
