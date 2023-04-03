import java.util.Scanner;
class A {
    int i;
}

class B {
    int main()
    {
        Scanner in = new Scanner(System.in);
        A a = new A();
        a.i = in.nextInt();
        for (int i = 0; i < a.i; i++) {  // Loop here
        }
    }
}
