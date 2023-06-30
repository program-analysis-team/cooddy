
void foo()
{
    int arr[10];
    int idx = 0;
    while (true) {
        arr[idx] = 0;  // no error here
        if (idx == 9) {
            break;
        }
        idx++;
    }
}
