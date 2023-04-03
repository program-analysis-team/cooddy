import java.util.Scanner;

class GetInputFromUser {
    int ReadSize()
    {
        Scanner in = new Scanner(System.in);
        int a = in.nextInt();
        return a + 10;
    }

    int main()
    {
        int a = ReadSize();
        int b = 1000 / a;
        return 0;
    }
}
