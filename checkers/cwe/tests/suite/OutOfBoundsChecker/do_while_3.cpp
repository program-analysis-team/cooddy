int get_index();

int foo()
{
    int arr[0xBB];
    int i = 0;
    int to = get_index();
    do {
        arr[i++];
    } while (i < to);
    return 0;
}
