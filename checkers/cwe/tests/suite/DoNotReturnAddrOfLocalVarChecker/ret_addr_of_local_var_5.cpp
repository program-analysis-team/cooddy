struct DDP_WRITE_CTX_S {
    int *scheduleFrame;
    void (*pfnExcute)(void *);
};

void bucketSplitExcuDBG(DDP_WRITE_CTX_S *cntxData)
{
    int scheduleFrame;
    cntxData->scheduleFrame = &scheduleFrame;
    cntxData->pfnExcute(0);
    (void *)&scheduleFrame;
}
