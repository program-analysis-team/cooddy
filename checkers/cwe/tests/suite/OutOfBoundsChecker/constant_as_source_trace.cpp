
unsigned get_index(bool flag)
{
    unsigned ret = 0;
    if (flag) {
        ret = 10;  // right 'source' in trace
    }
    return ret;
}

void foo(bool flag)
{
    int arr[10];
    auto idx = get_index(flag);
    arr[idx];  // error here
}
