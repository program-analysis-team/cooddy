import java.util.Scanner;
class A {
    int foo()
    {
        Scanner in = new Scanner(System.in);
        int a = in.nextInt();
        int b = a - 10;
        int c = 0 / b;   // sink 1
        int d = 10 % c;  // sink 2
        return b + c;
    }
}
