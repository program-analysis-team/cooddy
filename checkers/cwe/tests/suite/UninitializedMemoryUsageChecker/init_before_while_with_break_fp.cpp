
int some_func();

int check(int v)
{
    if (v == 123) {
        return 1;
    }
    return 0;
}

int init(int *p, int v)
{
    int ret = check(v);
    if (ret != 0) {
        return ret;
    }
    *p = 10;

    int retry = 10;
    while (retry-- > 0) {
        if (!some_func()) {
            continue;
        }
        if (!some_func()) {
            continue;
        }
        ret = 0;
        break;
    }
    return ret;
}

int foo(int v)
{
    int p;
    int ret = init(&p, v);
    if (ret == 0) {
        return p;
    }
    return 0;
}
