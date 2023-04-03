import java.util.Scanner;
class B {
    int c;
}
class A {
    B bar1(String str)
    {
        B b = new B();
        b.c = Integer.parseInt(str);
        return b;
    }
    int main()
    {
        Scanner in = new Scanner(System.in);
        int x = in.nextInt();
        String[] arr = new String[bar1(x).c];  // error here
    }
}
