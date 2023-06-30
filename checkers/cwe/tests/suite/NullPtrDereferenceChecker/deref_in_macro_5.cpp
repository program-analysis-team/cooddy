#define BOO(a, b, c) ((a) + *(b) + (c))

void foo()
{
    int* x = nullptr;
    BOO(42, x, 0);
}
