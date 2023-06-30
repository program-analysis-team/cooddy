#include "string"
class Object {
public:
    std::string property;
    int count;
};

int main()
{
    Object Obj1;
    Obj1.property = "B";
    Obj1.count = 19;

    Object Obj2;
    Obj2.property = "F";
    Obj2.count = 38;

    int a = Obj1.count * 2 - Obj2.count;
    int z = Obj1.count / a;
    return 0;
}
