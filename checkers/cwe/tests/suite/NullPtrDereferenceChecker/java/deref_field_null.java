
class A {
    String m_f;
    protected int flag;
};

class B {
    int main()
    {
        A a = new A();
        A b;
        a.m_f = null;
        b = a;
        return b.m_f.length();
    }
}
