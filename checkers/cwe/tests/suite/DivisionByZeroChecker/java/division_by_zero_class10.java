package p1;
class C {
  C() {
    zero = 5;
  }
  int foo() {
    A a = new A();
    final int k = 1/zero;
  }
  final long zero;
}
