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
    if (x == (long int) x || x == INFINITY || x == -INFINITY) return x;
    return (x > 0) ? (long int) x : (long int) (x-1);
}

double customCeil(double x) {
    if (x == (long int) x || x == INFINITY || x == -INFINITY) return x;
    return (long int) (x+1);
}

// TODO: implement square root
double customSqrt(double x){
    return sqrt(x);
}
