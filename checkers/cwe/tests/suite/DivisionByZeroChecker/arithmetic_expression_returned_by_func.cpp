#include <iostream>

using namespace std;

int ReadSize()
{
    int a;
    cin >> a;
    return a + 10;
}

int main()
{
    int a = ReadSize();
    int b = 1000 / a;
    return 0;
}
