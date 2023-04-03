
int* bar();

int foo(int v)
{
    auto p = bar();
    int a = *p;
    if (v == 10 && p == nullptr) {
        return 1;
    }
    return 0;
}
