
void foo()
{
    int arr[16];
    arr[(16 + 12) & 15] = 0;  // no error

    for (int i = 0; i < 64; ++i) {
        arr[i >> 2] = 0;  // no error
    }
}
