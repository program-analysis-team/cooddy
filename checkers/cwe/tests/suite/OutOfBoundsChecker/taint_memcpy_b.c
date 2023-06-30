int taint();

void foo()
{
    int arr[10] = {};
    int idx1 = taint();
    int idx2 = 0;

    memcpy(&idx2, &idx1, sizeof(int));
    arr[idx2] = 0; // error here
}
