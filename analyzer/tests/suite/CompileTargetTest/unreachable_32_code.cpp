int bar()
{
#if defined(__i386__)
    int *x = nullptr;
    return *x;
#endif
    // COODDY_SUPPRESS ReturnConstantValueChecker
    return 42;
}
