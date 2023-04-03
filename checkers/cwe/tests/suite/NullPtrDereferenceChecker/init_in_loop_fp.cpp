
int bar(int** p)
{
    static int a[10];
    int i = 0;
    for (i = 0; i < 10; i++) {
        if (a[i] == i) {
            *p = &a[i];
            break;
        }
    }
    if (i == 10) {
        return 1;
    }
    return 0;
}

int foo()
{
    int* p = 0;
    int ret = bar(&p);
    if (ret == 0) {
        return *p;  // no NPD here
    }
    return 0;
}
