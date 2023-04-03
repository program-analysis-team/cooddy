#define MACRO(a, b) (b)

#define MACRO_DO(a, b) \
    do {               \
        MACRO(a, b);   \
    } while (0)

void foo()
{
    int* p = 0;
    MACRO_DO(10, *p);
}
