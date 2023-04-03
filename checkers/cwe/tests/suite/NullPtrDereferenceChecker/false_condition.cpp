
int main()
{
    int *p1;
    int *p2;
    if (false)
        p1 = 0;
    if (true)
        p2 = 0;
    int c = *p1;  // no error here
    return *p2;   // error here
}
