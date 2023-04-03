
class Simple{
    public static int foo(int v) {
 	String p = null;
        if (v == 1 && (p == null || p.length() == 1))
 	        return 0;
 	return 1;
    }

    public static int foo1(int v) {
 	String p = null;
        if (p != null && v != 1 && p.length() == p.length())
 	        return 0;
 	return 1;
    }
}
