
int* get_work(int*& pool)
{
    pool = 0;
}

void func(int* pool)
{
    int* work = (int*)10;
    get_work(work);
    int c = *work;  // error here
}
