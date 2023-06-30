typedef struct MsgHdr_ {
    char id;
} MsgHdr;

typedef struct MsgA_ {
    char id;
    char name[12];
    long type;
} MsgA;

typedef struct MsgB_ {
    char id;
    char name[12];
} MsgB;

void check_struct_overflow(MsgHdr* msg) {
    switch(msg->id)
    {
        case 5:
            MsgA *msgA = (MsgA*) msg;
            msgA->type = 0; // Problem here: write out of bounds MsgB
            break;
    }
}

int main(int argc, char** argv){
    MsgB msgB;
    msgB.id = 5;
    check_struct_overflow((MsgHdr*)&msgB);
    return 0;
}
