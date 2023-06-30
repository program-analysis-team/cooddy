void* malloc(unsigned);

class AAA {
public:
    static AAA* instance;
    static AAA* GetInstance()
    {
        if (!instance) {
            instance = (AAA*)malloc(sizeof(AAA));
        }
        return instance;
    }
    void foo() {}
};

AAA* AAA::instance = nullptr;

class BBB {
public:
    static BBB* GetInstance()
    {
        static BBB* instance = nullptr;
        if (!instance) {
            instance = (BBB*)malloc(sizeof(BBB));
        }
        return instance;
    }
    void foo() {}
};

void foo()
{
    auto ptrAAA = AAA::GetInstance();
    ptrAAA->foo();
    auto ptrBBB = BBB::GetInstance();
    ptrBBB->foo();
}
