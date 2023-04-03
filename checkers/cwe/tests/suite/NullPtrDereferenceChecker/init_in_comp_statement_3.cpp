int foo() {
    int *ptr = nullptr;
    int x = ({42;});
    if (x) {
        return *ptr;
    }
    return 0;
}
