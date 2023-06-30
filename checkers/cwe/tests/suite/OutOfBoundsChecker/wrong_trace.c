#define MAX_NUM 16

int SomeFunc(int *nodeArr, int maxNum, int *num);
void *SomeMalloc(int size);

typedef struct {
    int nodeNum;
    int nodeArr[MAX_NUM];
    int dummy;
} NodeStruct;

int Bar(int nodeNum, int *nodeArr)
{
    int ret = 0;
    for (int nodeIndx = 0; nodeIndx < nodeNum; nodeIndx++) {
        ret += nodeArr[nodeIndx];
    }
    return ret;
}

int Foo()
{
    NodeStruct *node = SomeMalloc(sizeof(NodeStruct));
    if (node == 0) {
        return 1;
    }
    int tmpNum;
    int ret = SomeFunc(node->nodeArr, MAX_NUM, &tmpNum);
    if (ret != 0) {
        return 1;
    }
    node->nodeNum = tmpNum;
    ret = Bar(node->nodeNum, node->nodeArr);
    return ret;
}
