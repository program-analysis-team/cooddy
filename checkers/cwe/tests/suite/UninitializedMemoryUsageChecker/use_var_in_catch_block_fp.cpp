#include <exception>

int foo()
{
    int a;
    try {
        a = 10;
    } catch (std::exception &ex) {
        ex.what();
    }
    return a;
}
