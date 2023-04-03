
int func(int** p)
{
    *p = 0;
}

int main()
{
    int* a;
    func(&a);
    return *a;
}
