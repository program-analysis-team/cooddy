#include <cassert>
#include <cstdint>
#include <cstdio>
inline void rtl_check(char const* message, char const* filename, size_t line)
{
    assert(0);
}
#define RTL_CHECK(Expression) (void)((!!(Expression)) || (rtl_check(#Expression, __FILE__, __LINE__), 0))

namespace Wsf {

class Workspace {
public:
    static int* GetService(int id)
    {
        static int g_var;
        if (id > 1000) {
            RTL_CHECK(false);
            return nullptr;
        }
        return &g_var;
    };
};

};  // namespace Wsf

int foo(int v)
{
    int* p = Wsf::Workspace::GetService(20);
    return *p;  // no NPD here
}
