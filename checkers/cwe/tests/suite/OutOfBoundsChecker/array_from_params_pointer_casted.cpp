#include <memory>

auto func_64(const uint64_t target[2])
{
    return (*(const uint64_t*)target);
}
void func_8(const uint8_t t[2])
{
    func_64((const uint64_t*)(const void*)t);
}
