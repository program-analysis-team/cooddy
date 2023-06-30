public class Sample {
    public static void main(String[] args)
    {
        // Using Console to input data from user
        String name = System.console().readLine();
        ProcessBuilder pb = new ProcessBuilder(name);
        Process p = pb.start();
    }
}
