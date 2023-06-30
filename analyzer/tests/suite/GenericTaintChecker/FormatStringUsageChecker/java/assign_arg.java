import java.util.Scanner;
class Holder<T> {
    public Holder(T value) {
        this.value = value;
    }
    public T value;
}
public class FormatExample {
    A(Holder<String> output) {
        Scanner in = new Scanner(System.in);
        output.value = in.nextLine();
    }
    public static void main() {
        Holder<String> h = new Holder("");
        A(h);
        String sf1 = String.format(h.value, 1, 2, 3);
    }
}
