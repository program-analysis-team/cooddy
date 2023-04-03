
int get(int val, int *v)
{
    int p;
    int ret;
    ret = val != 1 ? 0 : 123;
    if (ret != 0)
        return ret;
    for (p = 0; p < val; p++) {
    }
    *v = p;
    return ret;
}

int foo(int val)
{
    int p;
    int rc = get(val, &p);
    if (rc != 0) {
        return rc;
    }
    if (p == val) {
        return p;
    }
    return 0;
}
