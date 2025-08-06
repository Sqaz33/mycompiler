a = 0;
b = 1;
x = input;
n = 0;

while (n < x) {
    n = n + 1;
   
    if (n == 1) {
        print a;
    }

    if (n == 2) {
        print b;
    }

    if (n > 2) {
        buf = b;
        b = a + b;
        a = buf;
        print b;
    }
}