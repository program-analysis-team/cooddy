class Main {
    void foo_1() {
        String s = null;
        int x = 6;
        if (x > 10) {
            return;
        } else if (x > 5){
            s.Length(); // NPD in else-if branch
        } else {
            return;
        }
    }
    void foo_2() {
        String s = null;
        int x = 4;
        if (x > 10) {
            return;
        } else if (x > 5){
            return;
        } else {
            s.Length(); // NPD in else-if-else branch
        }
    }
}
