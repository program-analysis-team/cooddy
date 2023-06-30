struct A {
    int b;
    A* foo() {return nullptr;}
};

A* GetA() {
    return nullptr;
}
void b() {
    *GetA(); //FN here
}
void c() {
    GetA()->b; //FN here
}
void d() {
    GetA()[10]; //FN here
}
void e() {
    GetA()->foo(); //FN here
}
A* a;
void f() {
    a->foo()->b; //FN here
}
