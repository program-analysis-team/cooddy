void Foo(int& par)
{
    auto func = [&](int& it) {
        int numArgs = it + 3;
        if (numArgs != 0) {
            it = 5;
        }
    };
}
