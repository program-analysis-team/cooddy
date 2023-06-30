#include <cstdint>
#define SZ 42

uint32_t _resultIsEq42();
uint32_t _resultIsNe42();
uint32_t _emptyCondition();
uint32_t _emptyCondition();
void _setPtrValue(int*);
uint32_t _ite_or(int, int);
uint32_t _ite_and(int, int);
uint32_t _priority();

uint32_t _add(int, int, int);
uint32_t _sub(int, int);
uint32_t _mul(int, int);
uint32_t _div(int, int);
uint32_t _rem(int, int);

uint32_t arr[SZ];

void f1()
{
    if (_resultIsEq42() == 42) {
        return;
    }
    arr[100];  // not reachable
}

void f2()
{
    if (_resultIsNe42() == 42) {
        return;
    }
    arr[100];  // OOB
}

void f3()
{
    auto idx = _emptyCondition();
    arr[idx];  // OOB, _emptyCondition is undefined
}

void f4_add()
{
    auto idx = _add(1, 10, 30);
    arr[idx];  // idx=41, OK
    idx = _add(1, 10, 40);
    arr[idx];  // idx=51, OOB
}

void f4_sub()
{
    auto idx = _sub(100, 60);
    arr[idx];  // idx=40, OK
    idx = _sub(100, 20);
    arr[idx];  // idx=80, OOB
}

void f4_mul()
{
    auto idx = _mul(11, 3);
    arr[idx];  // idx=33, OK
    idx = _mul(11, 4);
    arr[idx];  // idx=44, OOB
}

void f4_div()
{
    auto idx = _div(24, 3);
    arr[idx];  // idx=8, OK
    idx = _div(1000, 2);
    arr[idx];  // idx=500, OOB
}

void f4_rem()
{
    auto idx = _rem(100, 30);
    arr[idx];  // idx=10, OK
    idx = _rem(1000, 300);
    arr[idx];  // idx=200, OOB
}

void f5()
{
    int* ptr = nullptr;
    _setPtrValue(ptr);
    arr[*ptr];
}

void f6_ite_or()
{
    auto idx = _ite_or(0, 2);
    arr[idx];  // OOB
    idx = _ite_or(1, 0);
    arr[idx];  // OOB
    idx = _ite_or(0, 0);
    arr[idx];  // OK
}

void f6_ite_and()
{
    auto idx = _ite_and(1, 2);
    arr[idx];  // OOB
    idx = _ite_and(1, 0);
    arr[idx];  // OK
    idx = _ite_and(0, 1);
    arr[idx];  // OK
    idx = _ite_and(0, 0);
    arr[idx];  // OK
}

void f7_priority()
{
    if (_priority() == 20) {
        return;
    }
    arr[100];  // not reachable
}

int _free_symbol_cond(int*);

void f8_free_symbol_cond_0() {
    int arr[100];
    int x = 420;
    int ret = _free_symbol_cond(&x);
    if (ret != 0) {
        return;
    }
    arr[x];
}

void f8_free_symbol_cond_1() {
    int arr[100];
    int x = 420;
    int ret = _free_symbol_cond(&x);
    if (ret == 0) {
        return;
    }
    arr[x];
}
