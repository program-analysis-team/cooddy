class A {
public:
    int GetSize();
};

int main()
{
    A* ptr = nullptr;
    int x = 0;
    if (x == 1) {                         //  The code is unreachable
        auto y = sizeof(ptr->GetSize());  //  Null pointer is dereferenced inside compile time expression
    }
    auto z = ptr->GetSize();  //	Actual null pointer dereference we should detect
}
