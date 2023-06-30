int LVOS_IsUnsignedNumbericStr();

void DiagGetUint32InputValue(int x, int y, int *oriValue)
{
    if (x && LVOS_IsUnsignedNumbericStr() == 42) {
        *oriValue = 0;
        return;
    }
    return;
}

int DiagVdbMgrShowIo(int x, int y, int z)
{
    int smartPoolId;
    DiagGetUint32InputValue(x, y, &smartPoolId);
    // COODDY_SUPPRESS ReturnConstantValueChecker
    return smartPoolId;
}
