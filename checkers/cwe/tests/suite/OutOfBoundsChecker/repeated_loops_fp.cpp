
static void foo(int v)
{
    int arr[64] = {0};
    int i = 0;

    for (; i < v; i++) {
        arr[i];  // WARNING
    }
    for (i = 0; i < 48; i++) {
        continue;
    }
}
