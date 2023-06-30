int Foo1(int x) {
    static_assert(true, "It is first text");
    return x;
}

int Foo2(int x) {
    static_assert(false, "It is second text");
    return x;
}

int Foo3(int x) {
    static_assert(false, "It is first text");
    return x;
}

int Foo4(int x) {
    static_assert(true, "It is second text");
    return x;
}

int Foo5(int x) {
    static_assert(true);
    return x;
}

int Foo6(int x) {
    static_assert(true);
    return x;
}

int Foo7(int x) {
    static_assert(true, "");
    return x;
}
