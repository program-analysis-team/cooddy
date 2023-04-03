import java.util.Scanner;
class A {
    int[] i = new int[10];
}
class B {
    int main()
    {
        Scanner in = new Scanner(System.in);
        A a = new A();
        a.i[5] = in.nextInt();
        for (int i = 0; i < a.i[5]; i++) {  // Loop here
        }
    }
}
