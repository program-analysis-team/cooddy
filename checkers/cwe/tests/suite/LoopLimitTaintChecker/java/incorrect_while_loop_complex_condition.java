import java.util.Scanner;

class A {
    int main()
    {
        Scanner in = new Scanner(System.in);
        int x = in.nextInt();
        int i = 0;
        while (2 * i < 2 * x + 3) {
            ++i;
        }
    }
}
