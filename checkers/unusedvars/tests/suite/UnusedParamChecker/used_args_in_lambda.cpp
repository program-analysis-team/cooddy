int test(int a)
{
    int k;
    auto cb = [&](int v) {
       k = v + a;
    };
    cb(123);
    return k;
}
