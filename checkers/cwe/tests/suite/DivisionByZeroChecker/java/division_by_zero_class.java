class A {
    String property;
    int count;
}
class B {
    int main()
    {
        A Obj1 = new A();
        Obj1.property = "B";
        Obj1.count = 19;

        A Obj2 = new A();
        Obj2.property = "F";
        Obj2.count = 38;

        int a = Obj1.count * 2 - Obj2.count;
        int z = Obj1.count / a;
        return 0;
    }
};
