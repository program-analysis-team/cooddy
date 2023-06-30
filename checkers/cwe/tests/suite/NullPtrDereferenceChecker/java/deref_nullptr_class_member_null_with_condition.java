class Foo {
    Foo()
    {
        myVar = null;
    }

    String GetVar()
    {
        return myVar;
    }

    int GetAnotherVar()
    {
        return myAnotherValue;
    }
    String myVar;
    int myAnotherValue = 5;
}
class B {
    int main()
    {
        Foo foo = new Foo();
        if (foo.GetAnotherVar() < 10) {
            return foo.GetVar().length();
        }
        return 0;
    }
}
