#include <sstream>
#include <string>

int main()
{
    std::stringstream ss;

    ss << 100 << ' ' << 200;
    int foo, bar;
    ss >> foo >> bar;

    return 0;
}
