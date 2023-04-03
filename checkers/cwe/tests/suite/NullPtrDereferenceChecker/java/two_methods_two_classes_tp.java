class Main{
    class B {
        void fff()
        {
            A a = new A();
            String c = null;
            a.foo(c);
        }
    }

    class A {
        public int foo(String x)
        {
            return x.length();  // NPD here
        }
    }
}
