#include <cwchar>

int foo()
{
    // Will find no problems if std::fwprintf is broken
    std::fwprintf(nullptr, L"", 1 / 0);
}
