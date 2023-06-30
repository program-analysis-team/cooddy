#include <utility>

template <typename BaseClass>
class A : public BaseClass {
public:
    template <class... Args>
    A(Args... args) : BaseClass(std::forward<Args>(args)...)
    {
    }

private:
    int value;
};
