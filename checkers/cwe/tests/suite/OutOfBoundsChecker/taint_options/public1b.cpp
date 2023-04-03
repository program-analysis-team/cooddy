// Error: using arg without checking in public api function
int f(int a)
{
    int arr[10] = {};

    return arr[a];
}
