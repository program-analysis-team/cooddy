
int some_func();

int bar2(int idx)
{
    int arr[10] = {0};
    return arr[idx];  // error here
}

int bar()
{
    int idx = some_func();
    return bar2(idx);
}
