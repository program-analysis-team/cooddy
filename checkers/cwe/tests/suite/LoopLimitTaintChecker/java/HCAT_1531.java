import java.util.Scanner;

class A {
    int main()
    {
        Scanner in = new Scanner(System.in);
        to_upper(in.nextLine(), in.nextInt());
    }
    char to_upper(String str, int len){
        for (int i = 0; i < len; i++) {
            str[i] = "A";
        }
        return str;
    }

}
