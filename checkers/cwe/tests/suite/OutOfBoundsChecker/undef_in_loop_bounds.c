
int some_func();

int foo(int v)
{
    int buff[10] = {0};
    int num = some_func();
    for (int i = 0; i < num; ++i) {
        buff[i];
    }
    return 0;
}
