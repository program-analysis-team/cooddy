
void foo()
{
    int arr[10];
    unsigned long long i = 9;
    while (i > 0) {
        arr[i] = i;
        i -= 4;  // underflow here
    }
}

void foo2()
{
    int arr[10];
    unsigned long long i = 9;
    while (i > 1) {
        arr[i] = i;
        i -= 4;  // no error here
    }
}