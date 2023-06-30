#include <cstring>

int memset_s(void *dest, int destsz, int ch, int count);

void vbs_get_fixed_qos_meta_convert() {
    char line[123][123];
    memset_s(line, sizeof(line), 0, sizeof(line));
    strlen(line[1]); //No Error here
}
