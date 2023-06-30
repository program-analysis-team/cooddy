#include <sstream>
#include <string>

int main()
{
    std::stringstream ss;

    ss << 100 << ' ' << 200;
    int foo;
    ss >> foo;
    int c = foo+3;

    return 0;
}
