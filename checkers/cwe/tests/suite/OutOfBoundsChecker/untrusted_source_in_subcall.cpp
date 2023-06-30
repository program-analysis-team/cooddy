
extern "C" int some_func(int* num);

void boo(int* arr, int num)
{
    for (int i = 0; i < num; i++)
        arr[i];
}

void foo()
{
    int arr[10] = {0};
    int num = 0;
    some_func(&num);
    boo(arr, num);
}
