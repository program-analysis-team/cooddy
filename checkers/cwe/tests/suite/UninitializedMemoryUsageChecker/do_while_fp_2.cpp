int* foo1(int x)
{
    if (x % 2) {
        return nullptr;
    } else {
        return &x;
    }
}

int foo()
{
    for (int i = 0; i < 10; ++i) {
        auto ptr = foo1(i);
        if (ptr == nullptr) {
            continue;
        }
        auto x = *ptr;
        do {
        } while (0);
        break;
    }
    return 42;
}
