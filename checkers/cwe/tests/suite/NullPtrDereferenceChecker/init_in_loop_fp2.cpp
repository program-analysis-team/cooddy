
int bar()
{
    static int a[10];
    int i = 0;
    for (i = 0; i < 10; i++) {
        if (a[i] != i) {
            continue;
        }
        a[i] = 10;
        break;
    }
    return 0;
}
