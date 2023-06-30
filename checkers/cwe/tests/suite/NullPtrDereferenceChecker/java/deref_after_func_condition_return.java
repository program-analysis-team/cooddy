class A {
    int getValue(int var)
    {
        return var;
    }

    int main()
    {
        String s = null;
        if (getValue(0) == 1) {
            return 0;
        }
        return s.length(); // problem is here
    }
}
