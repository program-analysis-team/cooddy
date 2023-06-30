namespace {

struct StructInTheAnonymousNamespace1 {
    int myValue = 0;
};

}  // namespace

namespace MyNameSpace1 {

class ClassA {
    int mySecondValue = 0;
};

namespace MyNameSpace2 {

class ClassB {
    int myThirdValue = 0;
};

}  //  namespace MyNameSpace2

namespace MyNameSpace1 {  //  We use the same name for nested namespace

class ClassC {
    int myForthValue = 0;
};

}  // namespace MyNameSpace1

}  // namespace MyNameSpace1

namespace {

struct StructInTheAnonymousNamespace2 {
    int myValue = 0;
};

}  // namespace
