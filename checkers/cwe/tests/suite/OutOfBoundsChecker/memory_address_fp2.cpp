
int g_arr1[] = {1, 2, 3};
int g_arr2[] = {1, 2, 3};

void foo(int x)
{
    int* arr = x ? g_arr1 : g_arr2;
    arr[0];  // no error here
}
