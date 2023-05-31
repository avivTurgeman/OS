//
// Created by alon on 5/31/23.
//
// from gpt, request: write me a function in C that gets Unsigned int and checks if it's a prime number.

#include "Part_A.h"

int isPrime(unsigned int num) {
    if (num < 2) {
        return 0;
    }

    unsigned int limit = (unsigned int) sqrt(num);

    for (unsigned int i = 2; i <= limit; i++) {
        if (num % i == 0) {
            return 0;
        }
    }

    return 1;
}
