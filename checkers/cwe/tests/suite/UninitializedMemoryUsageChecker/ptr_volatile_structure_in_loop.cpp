#include <stdio.h>
#include <stdlib.h>


typedef struct {
    unsigned short  csr;
    unsigned short  data;
}uninit_memory_access_010_s_001;

#define ERROR   0x1
#define READY   0x2
#define RESET   0x4

#define NDEVS   4

volatile uninit_memory_access_010_s_001  *uninit_memory_access_010_s_001_arr_gbl;
void uninit_memory_access_010_func_001(unsigned devno)
{
    uninit_memory_access_010_s_001_arr_gbl = (uninit_memory_access_010_s_001 *) malloc (5*sizeof(uninit_memory_access_010_s_001));
    if(uninit_memory_access_010_s_001_arr_gbl!=NULL)
    {
        uninit_memory_access_010_s_001_arr_gbl->csr = READY;
        if(devno < NDEVS)
            do
            {
                uninit_memory_access_010_s_001_arr_gbl->data = READY;
                if(uninit_memory_access_010_s_001_arr_gbl->csr & ERROR)
                {
                    uninit_memory_access_010_s_001_arr_gbl->csr = RESET;
                }
            }while( (uninit_memory_access_010_s_001_arr_gbl->csr & (READY | ERROR)) == 0);
    }
}

void uninit_memory_access_010 ()
{
    uninit_memory_access_010_func_001(5);
    if(uninit_memory_access_010_s_001_arr_gbl!=NULL)
    {
        ++uninit_memory_access_010_s_001_arr_gbl->data;/*Tool should detect this line as error*/ /*ERROR:Uninitialized Memory Access*/
        free((void *)uninit_memory_access_010_s_001_arr_gbl);
    }
}
