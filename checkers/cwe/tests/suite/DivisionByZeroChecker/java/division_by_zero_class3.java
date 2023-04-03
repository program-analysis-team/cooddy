package p1;
class A {
    int main()
    {
        B b = new B();
        return 1/b.a(this);
    }
    int a(A a) {
        return 1; //not used
    }
}
class B {
    int a(A a) {
        return 0; //this source
    }
};
