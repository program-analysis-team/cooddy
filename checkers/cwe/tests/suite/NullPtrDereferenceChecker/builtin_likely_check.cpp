int* foo()
{
    return nullptr;
}

int main()
{
    int* ptr = foo();
    if (__builtin_expect(ptr != nullptr, 0)) {
        return *ptr;
    }
    return -1;
}
