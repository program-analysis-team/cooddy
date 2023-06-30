package p1;
class B {
    int a(A a) {
        return 0; //not used
    }
}
class A {
    int main()
    {
        A b = new A();
        return 1/b.a(this);
    }
    int a(A a) {
        return 1; //this source
    }
}
