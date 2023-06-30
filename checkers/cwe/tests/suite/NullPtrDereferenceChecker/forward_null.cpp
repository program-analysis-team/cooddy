
int foo(int* p, int v)
{
    if (v == 10 || p == nullptr) {
        return *p;  // NPD here
    }
    return 0;
}
