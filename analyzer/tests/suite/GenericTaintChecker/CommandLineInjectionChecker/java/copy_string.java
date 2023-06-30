import java.util.Scanner;

class A {
    void test()
    {
        Scanner in = new Scanner(System.in);
        String s = in.nextLine();
        String s2 = s.substring(5, 15);
        Runtime rt = Runtime.getRuntime();
        Process pr = rt.exec(s2);

        ProcessBuilder pb = new ProcessBuilder(s2);
        Process p = pb.start();

        ProcessBuilder pb = new ProcessBuilder("app.exe", s2);
        Process p = pb.start();

        ProcessBuilder pb = new ProcessBuilder("app.exe", "arg1", s2);
        Process p = pb.start();
    }
}
