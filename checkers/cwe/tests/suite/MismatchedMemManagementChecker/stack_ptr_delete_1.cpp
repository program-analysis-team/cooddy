void foo2(int *ptr) {
    delete ptr;
}

void foo1() {
    int x;
    foo2(&x);
}
