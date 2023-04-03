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
    int b;
    if (a > 0) {
        b = 1000 / a;
    } else {
        b = 10;
    }
    return 0;
}
