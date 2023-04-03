package p1;
class C {
  int foo() {
    A a = new A();
    int k = 1/a.zero;
  }
}
class A {
    static final int zero = 0;
}
