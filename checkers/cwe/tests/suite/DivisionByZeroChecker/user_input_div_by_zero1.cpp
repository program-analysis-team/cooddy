#include <iostream>

using namespace std;

int ReadSize()
{
    int a;
    cin >> a;
    return a;
}

int main()
{
    int a = ReadSize();
    int b = 1000 / a;
    return 0;
}
