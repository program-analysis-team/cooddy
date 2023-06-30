void foo()
{
    int arr[0xBB];
    int i = 0;
    int to = 0xBBB;
    while (i < to) {
        int x = arr[i++];  // OOB
    }
}
