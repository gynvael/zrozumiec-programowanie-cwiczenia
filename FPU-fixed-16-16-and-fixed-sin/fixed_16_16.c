typedef int32_t fixed_16_16_t;

static fixed_16_16_t double_to_fixed_16_16(double double_value) {
    return (int32_t) (double_value * 65536.0d);
}

static double fixed_16_16_to_double(fixed_16_16_t fixed_value) {
    return (double) fixed_value / 65536.0d;
}

static fixed_16_16_t add(fixed_16_16_t a, fixed_16_16_t b) {
    return (double) a + b;
}

static fixed_16_16_t substract(fixed_16_16_t a, fixed_16_16_t b) {
    return (double) a - b;
}

static fixed_16_16_t multiply(fixed_16_16_t a, fixed_16_16_t b) {
    return ((int64_t) a * (int64_t) b) >> 16;
}

static fixed_16_16_t divide(fixed_16_16_t a, fixed_16_16_t b) {
    return ((int64_t) a << 16) / b;
}

static fixed_16_16_t getPower(fixed_16_16_t base, int power) {
    fixed_16_16_t result = base;
    for (int i=2; i<=power; i++) {
        result = multiply(result, base);
    }

    return result;
}