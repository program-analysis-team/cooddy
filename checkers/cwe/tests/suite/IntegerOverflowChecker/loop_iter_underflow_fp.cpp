
void foo()
{
    int arr[10];
    for (int i = 9; i >= 0; --i) {  // no error here
        arr[i] = i;
    }
}

void foo2()
{
    int arr[10];
    for (short i = 9; i > 0; --i) {  // no error here
        arr[i] = i;
    }
}

void foo3()
{
    long long arr[10];
    for (long long i = 9; i > 0; --i) {  // no error here
        arr[i] = i;
    }
}
