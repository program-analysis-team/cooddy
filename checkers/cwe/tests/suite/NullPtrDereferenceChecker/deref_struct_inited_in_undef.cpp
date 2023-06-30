struct A {
    char *ptr;
};

void some_func(A *a);

void foo() {
    A a = {0};
    some_func(&a);
    *a.ptr; // no error here
}
