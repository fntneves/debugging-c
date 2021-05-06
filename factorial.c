#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int factorial(int n) {
    int total = 1;

    while (n > 0) {
        total = total * n;
        n--;
    }

    return total;
}

int main(int argc, char * const argv[]) {
    int result = factorial(atoi(argv[1]));
    printf("factorial(%s) = %d\n", argv[1], result);

    return 0;
}
