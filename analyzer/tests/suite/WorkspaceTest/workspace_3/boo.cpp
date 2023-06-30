int main()
{
#ifdef NULLPTR_123
    int *p = 0;
    *p;
#endif

#ifdef DIVIDE_BY_ZERO
    int a = 0;
    int c = 123 / a;
#endif
    return 0;
}
