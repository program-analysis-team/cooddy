typedef unsigned char UINT8;
typedef unsigned int UINT32;

#define MAX_SCHED_USER_NUM_PER_TTI 34
#define MAX_USER_ID 32
#define NULL_PTR 0
#define min(a, b) ((a) < (b) ? (a) : (b))

typedef struct {
    int userType;
} SchrltStru;

typedef struct {
    UINT8 usrNum;
    SchrltStru usrList[MAX_USER_ID];
} PeriodSchUsrListStru;

static SchrltStru* NullReturn_BadCase_WhiteBox02(PeriodSchUsrListStru* periodSchList)
{
    SchrltStru* schrlt = NULL_PTR;
    UINT8 maxUsrNum = min(periodSchList->usrNum, MAX_SCHED_USER_NUM_PER_TTI);
    for (UINT32 usrIndex = 0; usrIndex < maxUsrNum; usrIndex++) {
        schrlt = &periodSchList->usrList[usrIndex];  // Defect here.
        if (schrlt->userType != 0) {
            continue;
        }
        return schrlt;
    }

    return NULL_PTR;
}
