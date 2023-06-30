
int foo(int* p, int v)
{
    if (v == 10 || !p) {
        return *p;  // NPD here
    }
    return 0;
}
