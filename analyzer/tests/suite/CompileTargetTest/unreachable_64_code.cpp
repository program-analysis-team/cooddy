int foo()
{
#if defined(__x86_64__)
    int *x = nullptr;
    return *x;
#endif
    // COODDY_SUPPRESS ReturnConstantValueChecker
    return 42;
}
