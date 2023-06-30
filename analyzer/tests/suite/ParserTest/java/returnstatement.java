public class Main {
    public int foo(int x, int y, boolean bx, boolean by) {
        if (bx) {
            return x + y;
        }
        return (y * 5);
    }
    public void foo(int x, int y, boolean bx, boolean by) {
        return;
    }
}
