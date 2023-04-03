import java.util.Scanner;
public class A {
    String v;
}
public class FormatExample {
    public static void main(String args[]) {
        Scanner in = new Scanner(System.in);
        A a = new A();
        a.v = in.nextLine();
        String sf1 = String.format(a.v, 1, 2, 3);
    }
}
