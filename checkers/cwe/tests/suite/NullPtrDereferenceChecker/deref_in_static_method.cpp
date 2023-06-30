struct MyTranslationUnit {
};

const int* GetMyTranslationUnit();

class CatchChecker {
public:
    //  It is required to reproduce the issue
    //  -   StaticFoo should be a static member. The issue disappears if StaticFoo is free function or non-static class
    //      member
    //  -   The function gets at least two parameters
    static bool StaticFoo(const int* trUnit, int someParam)
    {
        *trUnit;  //  False positive null pointer dereference is here!

        return true;
    }

    bool Foo() const
    {
        const int* trUnit = GetMyTranslationUnit();

        return StaticFoo(trUnit, 0);
    }
};
