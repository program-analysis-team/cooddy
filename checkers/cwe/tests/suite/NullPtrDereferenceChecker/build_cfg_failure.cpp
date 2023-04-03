#include <functional>
#include <string>
#include <unordered_map>

class BaseClass;

template <class T>
struct Foo {
    bool ForEach(const std::function<void(BaseClass&)>& func)
    {
        if (!func) {
            return false;
        }
        for (auto& keyItem : factory) {
            auto& objs = keyItem.second;
            for (auto& subKeyItem : objs) {
                func(*subKeyItem.second);
            }
        }
        return true;
    }

private:
    std::unordered_map<T, std::unordered_map<T, BaseClass*>> factory;
};

void bar()
{
    int* p = 0;
    Foo<std::string> foo;
    if (foo.ForEach(nullptr)) {
        *p = 0;  // no error here
    }
}
