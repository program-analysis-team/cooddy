
// address argument to atomic operation must be a pointer to
void atomic_builtin()
{
    _Atomic int a = 0;
    __atomic_load(&a, &a, 1);
}

// clang doesn't define GCC float types
void float_types()
{
    _Float32 a1 = 0;
    _Float32x a2 = 0;
    _Float64 a3 = 0;
    _Float64x a4 = 0;
    _Float128 a5 = 0;
}
