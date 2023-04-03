

int main(int v)
{
    int *p = 0;
    int *p1 = 0;
    for (int i = 0; i < 4; i++) {
        p = (int *)10;
    }
    int c = *p;  // no error here
    return *p1;  // error here
}
