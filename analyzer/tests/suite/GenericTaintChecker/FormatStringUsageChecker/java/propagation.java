import java.util.Scanner;

public class FormatExample {
    String A() {

    }
    public static void main() {
        Scanner in = new Scanner(System.in);
        String s = in.nextLine();
        s = s.substring(5,25);
        s = s.trim();
        s = s.split("*")[0];
        s = s.replaceFirst("*", "abc");
        s = s.replaceAll("*", "abc");
        s = s.replace('a', 'b');
        s = s.intern();
        s = s.concat("second");

        String sf1 = String.format(s, 1, 2, 3);
    }
}
