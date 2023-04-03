int foo1()
{
    int arr[0xBB];
    int i = 0;
    int to = 0xBB;
    do {
        arr[i++];  // no error here
    } while (i < to);
    return 0;
}

int foo2()
{
    int arr[0xBB];
    int i = 0;
    int to = 0xBBB;
    do {
        arr[i++];  // no error here
    } while (i < to && i < 0xBB);
    return 0;
}
