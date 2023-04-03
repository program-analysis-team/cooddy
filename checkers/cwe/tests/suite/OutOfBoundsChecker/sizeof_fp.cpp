struct A {
    char buff[10];
    int p;
} arr[2];

int main()
{
    for (int i = 0; i < sizeof(arr) / sizeof(arr)[0]; ++i)
        arr[i].p = 10;
    return 0;
}
