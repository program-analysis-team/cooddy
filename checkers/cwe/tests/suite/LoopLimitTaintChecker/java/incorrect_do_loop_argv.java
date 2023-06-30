import java.util.Scanner;

class A {
    int main()
    {
        Scanner in = new Scanner(System.in);
        int x = in.nextInt();
        int y = x + 1;
        int i = 0;
        do {
            i++;
        } while (i < y);  // Loop here
    }
}
