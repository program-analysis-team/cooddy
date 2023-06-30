#include <vector>
struct B {
};

void GetNode(B*& node)
{
    node = new B();
}

std::vector<B*> myNodes;

void generator()
{
    for (auto& it : myNodes) {
        GetNode(it);
    }
}
