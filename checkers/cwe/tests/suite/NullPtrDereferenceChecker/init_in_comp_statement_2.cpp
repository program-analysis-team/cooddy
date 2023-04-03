int bar() {
    return 42;
}

int foo() {
    int *ptr = nullptr;
    int x = ({int z = bar(); z;});
    if (x) {
        return *ptr;
    }
    return 0;
}
