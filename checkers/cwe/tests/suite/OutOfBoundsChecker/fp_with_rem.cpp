#include <cstdint>

#define TP_WORK_NUM (2)

int main(int argc, char* argv[])
{
    uint8_t arr[30];
    int32_t idx = 0;
    int32_t tmpIdx = (idx + 1) % TP_WORK_NUM;
    auto z = arr[tmpIdx];
    return 0;
}
