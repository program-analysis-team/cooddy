#define VOS_NULL_PTR 0L
#define RETURN_IF(expr, errcode) {if(VOS_NULL_PTR == expr) {return (errcode);}}
#define RETURN_IF2(expr, errcode) {if(expr == VOS_NULL_PTR) {return (errcode);}}
int foo(int* p)
{
    int a = *p;
    RETURN_IF(p, VOS_NULL_PTR); //FP
    return 0;
}
int foo2(int* p)
{
    int a = *p;
    RETURN_IF2(p, VOS_NULL_PTR); //FP
    return 0;
}
#define RETURN_IF3(expr, errcode) {if(expr) {return (errcode);}}

int foo3(int* p)
{
    int a = *p;

    RETURN_IF3(VOS_NULL_PTR == p, VOS_NULL_PTR); //TP
    return 0;
}
int foo4(int* p)
{
    int a = *p;
    RETURN_IF3(p == VOS_NULL_PTR, VOS_NULL_PTR); //TP
    return 0;
}
#define RETURN_IF4(expr, errcode) {if(!!expr) {return (errcode);}}

int foo5(int* p)
{
    int a = *p;

    RETURN_IF4(p, VOS_NULL_PTR); //TP
    return 0;
}
int foo6(int* p)
{
    int a = *p;
    RETURN_IF4((p), VOS_NULL_PTR); //TP
    return 0;
}
#define RETURN_IF5(expr, errcode) {if((expr) == (VOS_NULL_PTR)) {return (errcode);}}
#define RETURN_IF6(expr, errcode) {if((VOS_NULL_PTR) == (expr)) {return (errcode);}}
int foo7(int* p)
{
    int a = *p;
    RETURN_IF5(p, VOS_NULL_PTR); //FP
    return 0;
}
int foo8(int* p)
{
    int a = *p;
    RETURN_IF6(p, VOS_NULL_PTR); //FP
    return 0;
}

#define DEVPXY_RETURN_WHEN_NULL(ulRet, pPointer) \
    {                                            \
        if (VOS_NULL_PTR == (pPointer)) {        \
            return (ulRet);                      \
        }                                        \
    }
int foo9(int* p)
{
    int a = *p;
    DEVPXY_RETURN_WHEN_NULL(VOS_NULL_PTR, p); //FP
    return 0;
}
#define RETURN_IF7(expr, errcode) {if((expr) == (errcode)) {return (errcode);}}

int foo10(int* p)
{
    int a = *p;
    RETURN_IF7(p, VOS_NULL_PTR); //FP
    return 0;
}
