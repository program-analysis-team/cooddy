import java.util.Scanner;

class A {
    int main()
    {
        Scanner in = new Scanner(System.in);
        int x = in.nextInt();
        String[] arr = new String[x + 42];  // error here
    }
}
