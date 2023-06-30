
int foo(int *x)
{
    if (x) {
        return *x;
    }
    return *x;  // NPD here
}

void bar()
{
    foo(nullptr);
}
