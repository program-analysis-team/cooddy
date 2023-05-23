void foo()
{
    int arr[64];
    int a = 63;
    arr[63 ^ a] = 0;     // FP
    arr[128 ^ 128] = 0;  // FP
    arr[a | 63] = 0;     // FP
    arr[63 & a] = 0;     // FP
    arr[a << 0] = 0;     // FP
}