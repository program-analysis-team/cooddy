

extern "C" int boo(int v)
{
    int* p = 0;
    if (v == 1) {
        return *p;
    }
    int* p1 = 0;
    if (v == 2) {
        return *p1;
    }
    return 0;
}
