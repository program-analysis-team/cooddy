
void get_pool(int** pool);

int main()
{
    int* p = 0;
    get_pool(&p);
    return *p;
}
