struct A {
    char aucUserMac[6];
};

int main() {
    char aucTemp[264] = {0};
    A* pstKey = (A*)aucTemp;
    auto val = pstKey->aucUserMac[0]; //FP detected here
    return 0;
}
