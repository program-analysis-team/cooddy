
int untrustedSource();
int distComputeBatch = 2;

void sampleCode()
{
    int numVecs = untrustedSource();
    int newVecSize = numVecs + distComputeBatch;
    for (int i = 0; i < newVecSize; i++) {  // error here
    }
}
