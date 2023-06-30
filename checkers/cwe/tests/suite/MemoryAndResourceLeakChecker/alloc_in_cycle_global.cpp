#include <stdlib.h>
char *g_var[123];

void initFunc(int num)
{
    for (int i = 0; i < num; i++) {
        g_var[i] = (char *)malloc(0x124);
        if (g_var[i] == 0) {
            return;
        }
    }
}
