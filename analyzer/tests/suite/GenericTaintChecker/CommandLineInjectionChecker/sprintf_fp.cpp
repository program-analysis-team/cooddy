#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif

#define MAX_SCSI_DEV_CMD_LEN 100

int snprintf_s(char* buffer, int bufsz, int count, const char* format, ...);
#define dsw_snprintf_s(dst, dst_max, count, format, ...) snprintf_s(dst, dst_max, count, format, ##__VA_ARGS__)

struct S {
    char* s;
    int p;
};
int undef(S* s);

void fff(char* cmd)
{
    S s;
    s.s = cmd;
    undef(&s);
}

EXTER_ATTACK void vbm_handle_vbp_rescan_volume(int p)
{
    char cmd[MAX_SCSI_DEV_CMD_LEN];
    if (p > 10) {
        fff(cmd);
    }
    dsw_snprintf_s(cmd, MAX_SCSI_DEV_CMD_LEN, MAX_SCSI_DEV_CMD_LEN - 1, "sudo /opt/script_dir/script.sh");
}
