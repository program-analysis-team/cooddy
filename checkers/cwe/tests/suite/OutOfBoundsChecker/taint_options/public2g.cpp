// No error: internal linkage, not public api
static int f(int a)
{
    int arr[10] = {};

    return arr[a];
}
