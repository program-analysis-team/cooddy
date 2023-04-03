
int foo(int v, int* p)
{
    int a = *p;
    if (v == 10 && p == nullptr) {
        return 1;
    }
    return 0;
}
