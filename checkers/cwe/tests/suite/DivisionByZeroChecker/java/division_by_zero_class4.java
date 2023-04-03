package p1;
class C {
  int foo() {
    A a = new A();
    int k = 1/a.GetB().GetZero();
  }
}
class A {
  B GetB() {return new B();}
}
class B {
  int GetZero() {return 0;}
}
