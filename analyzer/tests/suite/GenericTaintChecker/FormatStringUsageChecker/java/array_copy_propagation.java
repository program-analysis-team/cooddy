import java.util.Scanner;

public class FormatExample {
    public static void main(String args[]) {
        Scanner in = new Scanner(System.in);
        String name = in.nextLine();
        String fmt = "";
        for(int i = 0; i < name.length(); i++)
            fmt += name[i];
        String sf1 = String.format(fmt, 1, 2, 3);
    }
}
