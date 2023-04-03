int memcpy_s(void* dstBuff, int dstMax, const void* srcBuff, int srcSize);
void HdcFastRecv(int* a);
unsigned int AAA();

class Vectors {
public:
    void addVectors(int size)
    {
        char dstBuff[20000];
        char srcBuff[20000];
        memcpy_s(dstBuff, size * sizeof(float), srcBuff, size * sizeof(float));
    }
};

void NoDetect2()
{
    Vectors v;
    int size = AAA();
    v.addVectors(size); //DETECT
}
