#include <iostream>

using namespace std;

int ReadSize()
{
    int a;
    cin >> a;
    if (a != -10) {
        return a + 10;
    }
    return 2;
}

int main()
{
    int a = ReadSize();
    int b = 1000 / a;
    return 0;
}
