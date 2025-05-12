#include <stdio.h>
#include "factorial.h"

int main() {
    int number = 5;
    int fact = factorial(number);
    printf("Factorial of %d = %d", number, fact);
    return 0;
}
