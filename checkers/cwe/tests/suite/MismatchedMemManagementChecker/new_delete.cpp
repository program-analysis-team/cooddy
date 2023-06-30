class BarObj {
public:
    BarObj() = default;
};

void foo()
{
    BarObj *ptr = new BarObj();
    delete ptr;
}
