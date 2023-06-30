class BarObj {
public:
    BarObj() = default;
};
void foo()
{
    BarObj *ptr = new BarObj[5];
    delete[] ptr;
}
