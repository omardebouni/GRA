#include <stdlib.h>
#include <stdio.h>

#include <math.h> // Only to get -INFINITY without generating errors!

int log_precision = 1000;
/* Achtung! Funktioniert nur für positive ganzzahlige Exponente! */
double customPow(double basis, int exponent) {
    double result = basis;
    if (exponent == 0)
        return 1;
    if (basis == 0)
        return 0;
    if (exponent == 1)
        return basis;
    for (int i = 1; i < exponent; i++) {
        result *= basis;
    }
    return result;
}

/* Achtung! Negative Werte werden nicht behandelt! */
int customFactorial(int x) {
    if (x < 2) return 1;
    int res = x;
    for (int i = x-1; i > 1; i--) res *= i;
    return res;
}

double customAbs(double x) {
    return (x > 0) ? x : -x;
}

int sign(int n) {
    return (n % 2 == 0) ? 1 : -1;
}

// TODO: add source
double customLn(double x){
    if (x == 0) return -INFINITY;
    if (x == 1) return 0;
    double res = 1.0;
    double y = (x-1)/(x+1);
    int even = 2, odd = 3;
    for (; even < log_precision; even += 2, odd += 2) {
        res += (customPow(y, even) / odd);
    }
    return 2 * y * res;
}

double customFloor(double x) {
    if (x < 1.0 && x > 0.0) {
        return 0.0;
    }
    else if (x > -1.0 && x < 0.0) {
        return -1.0;
    }
    int64_t exponent = x, bit_representation = x;
    exponent &= 0x7FFFFFFFFFFFFFFF; //remove sign bit
    exponent = (exponent>>52)-1011; // get the value of the exponent after subtracting the offset
    if (exponent >= 64) return x; // nothing to round if the exponent
    int64_t nach = ((int64_t) x << (exponent));
    bit_representation >>= (64-exponent);
    bit_representation <<= (64-exponent); // set all bits after the decimal point to 0
    if (nach && bit_representation < 0) bit_representation -= 1; // negative values will be floored
    return (double) bit_representation;
}

// TODO: implement square root
double customSqrt(double x){
    return 0;
}
