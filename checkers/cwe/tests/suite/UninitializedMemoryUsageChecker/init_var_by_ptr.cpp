void init(int* val) {
    *val = 0;
}

int main()
{
    int a;
    init(&a);
    int c = a + 1;
}
