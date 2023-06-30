#include <cstdint>
#include <stdlib.h>
struct SGL_S;
struct A {
    SGL_S* sgl;
};
struct SGL_S {
    A* entrySumInChain;
    int entrySumInSgl;
};

void allocSglChain(SGL_S** sglChain, uint32_t pageCnt, uint32_t moduleID, const char* funcName, const int32_t fileLine) {
    *sglChain = (SGL_S*)malloc(0x1234);
}

void cal_data_len_of_sgl(const SGL_S* data)
{
    auto val = data->entrySumInChain;
}

void stream_plog_append_sgl(const A* sgl_list)
{
    cal_data_len_of_sgl(sgl_list->sgl);
}

int32_t copyDataFromBufferToSgl(SGL_S* dstSgl);

void foo() {
    SGL_S* sgl;
    allocSglChain(&(sgl), 1234, 1234, "", 123);
    copyDataFromBufferToSgl(sgl);
    auto val = sgl->entrySumInChain;

}
