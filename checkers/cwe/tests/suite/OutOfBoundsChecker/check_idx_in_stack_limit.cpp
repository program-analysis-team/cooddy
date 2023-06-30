
int g_arr[10];

bool check_index(unsigned int idx)
{
    return idx < 10;
}

void foo4(unsigned int idx)
{
    if (check_index(idx)) {
        g_arr[idx] = 0;
    }
}

void foo3(unsigned int idx)
{
    foo4(idx);
}

void foo2(unsigned int idx)
{
    foo3(idx);
}

void foo1()
{
    foo2(10);
}
