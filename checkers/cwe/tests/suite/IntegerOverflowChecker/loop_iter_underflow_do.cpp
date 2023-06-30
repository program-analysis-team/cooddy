
void foo()
{
    int arr[10];
    unsigned short i = 9;
    do {
        arr[i] = i;
        i -= 4;  // underflow here
    } while (i > 0);
}

void foo2()
{
    int arr[10];
    unsigned short i = 9;
    do {
        arr[i] = i;
        i -= 4;  // no error
    } while (i > 1);
}
