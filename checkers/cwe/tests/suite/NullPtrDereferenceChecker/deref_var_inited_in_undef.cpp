void some_func(char** a);

void foo() {
    char* a = {0};
    some_func(&a);
    *a; // no error here
}
