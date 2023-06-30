
void HdcFastRecv(void** recvDataPtr);

class IndexSQ {
public:
    int distComputeBatch = 2;
    int addVectors(unsigned numVecs)
    {
        int newVecSize = numVecs + distComputeBatch;
        for (int i = 0; i < newVecSize; i++) {
        }
        return 0;
    }
};

class SessionContext {
public:
    IndexSQ* getIndex(unsigned indexId) const;
};

int sampleCode(SessionContext& context, int idxListSize, int indexId)
{
    void* ctrlBuf = nullptr;
    HdcFastRecv(&ctrlBuf);
    int* recvCtrlBuf = reinterpret_cast<int*>(ctrlBuf);
    int listSize = recvCtrlBuf[idxListSize];
    auto index = context.getIndex(indexId);
    IndexSQ* pIndex = dynamic_cast<IndexSQ*>(index);
    pIndex->addVectors(listSize);
    return 0;
}
