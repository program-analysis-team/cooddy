void foo(void*, void*);

void bar()
{
    int arr[10] = {0};
    int sz;
    foo(arr, &sz);
    for (int i = 0; i < sz; i++) {
        (void)arr[i];
    }
}
