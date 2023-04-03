#include <iostream>

using namespace std;

int ReadSize()
{
    int a;
    cin >> a;
    if (a > 0) {
        return a;
    }
    return -5;
}

int main()
{
    int a = ReadSize();
    int b = 1000 / a;
    return 0;
}
