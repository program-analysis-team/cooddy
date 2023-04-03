
int get_index(int v)
{
    int arr[10] = {0};
    for (int i = 0; i < 10; ++i) {
        if (arr[i] == v)
            return i;
    }
    return 0;
}

int foo(int v)
{
    int arr[10] = {0};
    int idx = get_index(v);
    return arr[idx];  // no error here
}
