
int init_def(int* p, int v)
{
    if (v == 13) {
        return 1;
    }
    *p = 10;
    return 0;
}

int init_undef(int* p);

int foo(int v)
{
    int p;
    int ret;
    if (v == 10) {
        ret = init_def(&p, v);
    } else {
        ret = init_undef(&p);
    }
    if (ret != 0) {
        return 0;
    }
    return p;
}
