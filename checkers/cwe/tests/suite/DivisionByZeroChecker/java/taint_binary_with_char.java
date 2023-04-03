import java.util.Scanner;
class A {
    int foo()
    {
        Scanner in = new Scanner(System.in);
        String s = in.nextLine();
        char c = s[0];
        int res = c - 'A';
        return 100 / res;
    }
}