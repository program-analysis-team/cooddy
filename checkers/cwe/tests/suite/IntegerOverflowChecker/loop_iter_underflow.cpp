
void foo()
{
    int arr[10];
    for (unsigned i = 9; i >= 0; --i) {  // underflow here
        arr[i] = i;
    }
}

void foo2()
{
    int arr[10];
    for (unsigned i = 9; i > 0; --i) {  // no error here
        arr[i] = i;
    }
}
