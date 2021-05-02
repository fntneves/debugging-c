#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int factorial(int n) {
    int total = 0;

    while (n > 0) {
        total = total * n;
    }

    return total;
}

int main(int argc, char * const argv[]) {
    int result = factorial(atoi(argv[1]));
    printf("factorial(%s) = %d\n", argv[1], result);

    return 0;
}
