class Simple{

    public static int foo(int v) {
 	String p = null;
        if (p == null)
           throw NullPointerException("NPD");
 	return p.length();
    }
}
