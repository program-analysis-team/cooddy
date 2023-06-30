int* foo()
{
    // COODDY_SUPPRESS ReturnConstantValueChecker
    return nullptr;
}

int bar(int f)
{
    if (f) {
        int* a = foo();
        return *a;
    }
    return 0;
}
