#include <cstddef>
#include <new>
#include <cstddef>

class Singleton {
public:
    static Singleton* some_new(std::size_t size)
    {
        return size < 10 ? new Singleton : nullptr;
    }

    static Singleton* get_instance()
    {
        static Singleton* factory = nullptr;
        if (factory == nullptr) {
            factory = some_new(100);
        }
        return factory;
    }

    int method();
};

int foo()
{
    auto f = Singleton::get_instance();
    return f->method();  // no NPD detect here
}
