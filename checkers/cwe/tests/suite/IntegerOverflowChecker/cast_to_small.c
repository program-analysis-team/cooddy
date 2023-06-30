#include <stdio.h>

typedef int int32_t;
typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;

void parm_Change_fun(uint32_t big_id, uint16_t id);

void parm_change()
{
    uint32_t big_id = 1000000000;
    uint16_t id = 0;
    // parm_Change_fen(old_id, id)
    parm_Change_fun(id, big_id);
    enum A {B, C, D};
    id = B; //FP here
    return;
}
