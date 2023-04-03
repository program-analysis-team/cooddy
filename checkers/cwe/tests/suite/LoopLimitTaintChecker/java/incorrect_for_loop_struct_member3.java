import java.util.Scanner;
class A {
    int i;
}
class C {
    A a = new A();
}
class B {
    int main()
    {
        Scanner in = new Scanner(System.in);
        C c = new C();
        c.a.i = in.nextInt();
        for (int i = 0; i < c.a.i; i++) {  // Loop here
        }
    }
}
