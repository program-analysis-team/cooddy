#include <cstdint>
#include <stdlib.h>
#include <string>


struct Condition {
    enum class Operation{
        NONE, EQ, NE, GT, LE, GE, LT, ACCESS, ACCESS_UNSINGED
    };

    void FromString(const std::string& str);
    std::string ToString() const;

    Operation myOperation = Operation::NONE;
    int32_t myOperand = 0;
};

int foo(int argc)
{
    Condition condition{Condition::Operation::ACCESS, 0};
    if (argc) {
        condition.myOperation = Condition::Operation::ACCESS_UNSINGED;
    }
    return 0;
}
