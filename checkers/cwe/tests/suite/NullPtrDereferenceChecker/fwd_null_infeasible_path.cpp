void foo1(int a, int* ptr)
{
    if ((a == 42 || a == 43) && ptr == nullptr) {
        return;
    }
    if (a == 42) {
        int x = *ptr;
    }
}

struct Node {
    Node* left;
    Node* right;
    int isBlack;
};

void foo2(Node* n)
{
    if ((n->left == nullptr || n->left->isBlack) && (n->right == nullptr || n->right->isBlack)) {
        // smt
    } else {
        if (n->right == nullptr || n->right->isBlack) {
            if (n->left) {
                // smt
            }
            n->left->right;
        }
    }
}

void foo3(int* p1, int* p2, int* p3)
{
    if (!((p1 != 0 && p2 != 0) && p3 != 0)) {
        return;
    }
    int x = *p3;
}

void foo4(int a, int b, int c, int* ptr)
{
    int x = ((a > 10 && a < 20) && c == 42) ? ((ptr != nullptr) ? 1 : 0) : 0;
    if (x) {
        *ptr;
    }
}
