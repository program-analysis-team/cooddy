package p1;
class D {
  int foo() {
    A a = new A();
    B b = a.GetB();
    C c = b.GetC();
    int zero = c.GetZero();
    int k = 1/zero;
  }
}
class A {
    B GetB() {return new B();}
}
class B {
    C GetC() {return new C();}
}
class C {
    int GetZero() {return 0;}
}
