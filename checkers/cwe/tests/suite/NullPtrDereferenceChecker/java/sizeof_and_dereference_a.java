class A {
    int main()
    {
        String pointer = null;
        int vInt = 0;
        if (vInt == 1) {  //  The code is unreachable
            pointer = "abc";
        }
        return pointer.length();
    }
}
