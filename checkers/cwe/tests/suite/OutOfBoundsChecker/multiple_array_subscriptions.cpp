
void foo(unsigned idx)
{
    int arr[10];
    if (idx > 10) {
        return;
    }
    if (arr[idx] == 0) {
        arr[idx] = 1;
    }
}
