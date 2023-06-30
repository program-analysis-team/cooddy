void free(void*);

void bar1(int* x)
{
    *x;
}

void bar1(int* arr, int i)
{
    free(arr);
    bar1(&arr[0]);
}

void bar2(int* arr, int i)
{
    free(arr);
    bar1(&arr[5]);
}

void bar3(int* arr, int i)
{
    free(arr);
    bar1(&arr[i]);
}
