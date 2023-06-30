#include <new>

struct Singleton {
    int method();
};

template <class T>
class Factory {
public:
    static Factory<T>* get_instance()
    {
        static Factory<T>* factory = nullptr;
        if (factory == nullptr) {
            factory = nullptr;
        }
        return factory;
    }

    T* Create();
};

int foo()
{
    auto f = Factory<Singleton>::get_instance();
    return f->Create()->method();  // no NPD detect here
}
