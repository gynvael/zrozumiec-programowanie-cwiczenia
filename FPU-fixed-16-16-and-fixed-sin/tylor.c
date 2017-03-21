#include <math.h>
#include "fixed_16_16.c"

static int getStrong(int x) {
    int result = 1;
    while (x != 0) {
        result *= x;
        x--;
    }

    return result;
}

static fixed_16_16_t taylor_fixed(fixed_16_16_t x) {
    return x - getPower(x, 3)/getStrong(3) + getPower(x, 5)/getStrong(5) - getPower(x, 7)/getStrong(7);
}

static double taylor_double(double x) {
    return x - pow(x, 3)/getStrong(3) + pow(x, 5)/getStrong(5) - pow(x, 7)/getStrong(7);
}

