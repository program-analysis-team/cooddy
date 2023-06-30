#include <cstdlib>
#include <new>

void* VOS_MemAlloc(size_t, void*, size_t size);
void* vosDynamicMemPtGet();

#define SPPT_NEW_OBJ(pid, className, ptr)                                                        \
    (VOS_NULL_PTR == ((ptr) = (className*)SPPT_MemAlloc(pid, sizeof(className)))) ? VOS_NULL_PTR \
                                                                                  : SPPT_NEW(ptr) className
#define SPPT_NEW(p) new (p)
#define SPPT_MemAlloc(pid, size) VOS_MemAlloc(pid, DYNAMIC_MEM_PT, size)
#define DYNAMIC_MEM_PT vosDynamicMemPtGet()
#define VOS_NULL_PTR 0L

class IqnmCpriMuxIdx2Session {
private:
    static IqnmCpriMuxIdx2Session* instance_;

public:
    void bar() {}
    IqnmCpriMuxIdx2Session() = default;
    static IqnmCpriMuxIdx2Session* Instance()
    {
        return (instance_ != VOS_NULL_PTR) ? instance_ : SPPT_NEW_OBJ(0, IqnmCpriMuxIdx2Session, instance_);
    }
};

int main(int argc, char** argv)
{
    return 0;
}
