struct A {
    char *ptr;
};

void some_func(A *a);

void foo() {
    A b = {0};
    A* a = &b;
    some_func(a);
    b.ptr; // no error here
}
