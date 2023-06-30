bool IsEven(int x) {
    if (x % 2 == 0) {
        goto labelEvens;
    }

    goto labelOdds;

labelEvens:
    return true;

labelOdds:
    return false;
}
