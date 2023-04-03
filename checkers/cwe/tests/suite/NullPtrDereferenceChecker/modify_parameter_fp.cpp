
void func(int* p)
{
    p = 0;
}

int main()
{
    int* v = 0;
    int* p = (int*)10;
    func(p);
    if (p == 0) {
        return *v;  // no error here
    }
    return *p;
}
