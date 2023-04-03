#define MACRO(a, b) (b)

int foo()
{
    int* p = 0;
    return MACRO(10, *p);
}
