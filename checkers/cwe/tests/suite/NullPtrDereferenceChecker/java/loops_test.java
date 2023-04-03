class Main {
    int foo_while_1() {
        String s = null;
        int i = 0;
        while (i < 10) {
            return 0;
        }
        return s.length(); // no npd here
    }

    int foo_while_2() {
        String s = null;
        int i = 0;
        while (i < 10) {
            i++;
        }
        return s.length(); // npd here
    }

    int foo_while_3() {
        String s = null;
        int i = 20;
        while (i < 10) {
            return 0;
        }
        return s.length(); // npd here
    }

    int foo_do_while_1() {
        String s = null;
        do {
            return 0;
        } while (true);
        return s.length(); // no npd here
    }

    int foo_for_1() {
        String s = null;
        for(int i=0; i<10; i++) {
            return 0;
        };
        return s.length(); // no npd here
    }

    int foo_for_2() {
        String s = null;
        boolean b = false;
        for(int i=0; i<10 && b; i++) {
            return 0;
        };
        return s.length(); // npd here
    }
}
