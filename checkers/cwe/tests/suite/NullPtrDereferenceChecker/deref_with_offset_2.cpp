void bar1(char* arr, int x)
{
    auto y = *(arr + x);
}

void foo1()
{
    char* arr = nullptr;
    bar1(arr, 10);
}

void bar2(char* arr, int* x)
{
    auto y = *(arr + *x);
}

void foo2()
{
    char* arr = nullptr;
    int* x = new int(10);
    bar2(arr, x);
    delete x;
}

void bar3(char* arr, int x)
{
    for (int i = 0; i < x; i++) {
        auto y = *(arr + i);
    }
}

void foo3()
{
    char* arr = nullptr;
    bar3(arr, 10);
}
