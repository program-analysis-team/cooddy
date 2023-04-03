package p1;
class C {
  int foo() {
    int k = 1/A.b.zero;
  }
}
class A {
    static B b = new B();
}
class B {
    static int zero = 0;
}
