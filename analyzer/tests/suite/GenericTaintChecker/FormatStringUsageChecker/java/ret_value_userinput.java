import java.util.Scanner;

public class FormatExample {
    String A() {
        Scanner in = new Scanner(System.in);
        return in.nextLine();
    }
    public static void main() {
        String sf1 = String.format(A(), 1, 2, 3);
    }
}
