int foo()
{
    int arr[0xBB];
    int i = 0;
    int to = 0xBBB;
    do {
        arr[i++];
    } while (i < to);
    return 0;
}
