
int* bar();

int foo(int v)
{
    auto p = bar();
    if (v == 10 || p == nullptr) {
        return *p;  // NPD here
    }
    return 0;
}
