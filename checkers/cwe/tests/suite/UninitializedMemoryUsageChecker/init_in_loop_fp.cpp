
void init_in_loop(int idx, int size, int* arr)
{
    for (int i = idx; i < size; ++i) {
        arr[i] = 10;
    }
}

int foo()
{
    int res;
    init_in_loop(0, 1, &res);
    return res;
}
