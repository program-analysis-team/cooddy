
struct A {
};

A* GetInstance() {
    static A* a = new A();
    return a;
}

void foo() {
    GetInstance(); // no error
}
