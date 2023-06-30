#define max(a, b) ((a) > (b) ? (a) : (b))

int main()
{
    int* p = nullptr;
    return max(*p, 678);
}
