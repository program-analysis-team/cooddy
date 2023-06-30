namespace MyNameSpace {

struct Interface {
    virtual bool VirtualFooA(int a) const = 0;
    virtual bool VirtualFooB(int b) = 0;
};

namespace {

class MyClass : public Interface {
private:
    virtual bool VirtualFooA(int a) const override { return a > 7 ? false : true; }
    bool VirtualFooB(int b) {
        if (b <= 10) {
            return true;
        }

        return false;
    }

public:
    static MyClass* Create() { return new MyClass(); }
};

}  //  namespace

}  // namespace MyNameSpace
