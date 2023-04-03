
struct Foo {
    Foo *next;
} root;

int main()
{
    Foo *p = 0;
    Foo *n;
    p = &root, n = p->next;
    return 0;
}
