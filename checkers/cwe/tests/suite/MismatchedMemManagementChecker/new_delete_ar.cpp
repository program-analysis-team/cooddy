class BarObj {
public:
    BarObj() = default;
};
void foo()
{
    BarObj *ptr = new BarObj[5];
    int c =123;
    delete ptr;
}
