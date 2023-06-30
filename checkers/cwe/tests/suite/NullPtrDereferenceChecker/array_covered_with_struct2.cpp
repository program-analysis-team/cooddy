struct A {
    char aucUserMac[6];
};

int main() {
    char c = 0;
    char aucTemp[264] = {c};
    A* pstKey = (A*)aucTemp;
    auto val = pstKey->aucUserMac[0]; //FP detected here
    return 0;
}
