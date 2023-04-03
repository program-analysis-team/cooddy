class Main {
    void foo_1() {
        String s = null;
        int x = 2;
        switch (x) {
            case (1):
            case (0):
                return;
            case (2):
                break;
        }
        s.Length();
    }

    void foo_2() {
        String s = null;
        int x = 2;
        switch (x) {
            case (1):
                break;
            default:
                s.Length();
        }
    }

    void foo_3() {
        String s = null;
        int x = 1;
        switch (x) {
            case (1):
            case (2):
                s.Length();
                break;
            default:
                break;
        }
    }
}
