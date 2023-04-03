package p1;
class A {
    int main()
    {
        B b = new B();
        return 1/b.b(this);
    }
    int a() {
        return 0;
    }
}
class B {
    int b(A a) {
        return a.a();
    }

};
