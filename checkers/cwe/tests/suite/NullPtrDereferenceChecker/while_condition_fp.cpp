
int* get_work(int* pool)
{
    if (!pool)
        return 0;
    return (int*)10;
}

void func(int* pool)
{
    int* work = get_work(pool);

    while (work != 0) {
        *work = 10;
        work += 10;
    }
}
