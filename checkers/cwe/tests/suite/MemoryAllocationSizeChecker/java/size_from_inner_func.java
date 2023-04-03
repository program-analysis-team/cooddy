import java.util.Scanner;

class A {
    int bar1(String str)
    {
        int x = Integer.parseInt(str);
        return x;
    }
    int main()
    {
        Scanner in = new Scanner(System.in);
        String x = in.nextLine();
        int sz = bar1(x);
        String[] arr = new String[sz];  // error here
    }
}
