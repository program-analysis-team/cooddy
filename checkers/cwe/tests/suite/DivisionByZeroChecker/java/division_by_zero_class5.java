package p1;
class D {
  int foo() {
    A a = new A();
    int k = 1/a.GetB().GetC().GetZero();
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
