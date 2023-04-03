class A {
    String m_f;
};

class B {
    int func(A a)
    {
        return a.m_f.length(); //Error here
    }

    int main()
    {
        A a = new A();
        a.m_f = null;
        return func(a);
    }
}
