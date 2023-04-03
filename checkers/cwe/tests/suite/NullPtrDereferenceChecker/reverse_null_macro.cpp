#define M(p)                \
    do {                    \
        if (p != nullptr) { \
        }                   \
    } while (0)
int foo(int v, int* p)
{
    int a = *p;
    M(p);
    return 0;
}
