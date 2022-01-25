#include "custom_math.h"
#include <math.h> // Only to get -INFINITY without generating errors!

#define LOG_PRECISION  1000
/* Achtung! Funktioniert nur für positive ganzzahlige Exponente! */
double customPow(double basis,int exponent) {
    if (exponent == 0)
        return 1;
    if (basis == 0)
        return 0;
    if (exponent == 1)
        return basis;

    double result = basis;
    while (--exponent > 0){
        result *= basis;
    }
    return result;
}

/* Achtung! Negative Werte werden nicht behandelt! */
long long customFactorial(int x) {
    if (x < 2) return 1;
    long long res = x, i = x-1;
    while (i > 1) {
        res *= i--;
    }
    return res;
}

double customAbs(double x) {
    return (x > 0) ? x : -x;
}

/* Returns +1 if n is even, -1 if n is odd */
long long sign(int n) {
    return (n % 2 == 0) ? 1 : -1;
}

// TODO: add source
double customLn(double x){
    if (x == 0) return -INFINITY;
    if (x == 1) return 0;
    double res = 1.0;
    double y = (x-1)/(x+1);
    int even = 2, odd = 3;
    for (; even < LOG_PRECISION; even += 2, odd += 2) {
        res += (customPow(y, even) / odd);
    }
    return 2 * y * res;
}

/* Babylonian method: https://www.geeksforgeeks.org/square-root-of-a-perfect-square/ */
double customSqrt(double n){
    double x = n;
    double y = 1;
    double e = 0.000001; // precision level
    while(x - y > e) {
        x = (x + y) / 2;
        y = n / x;
    }
    return x;
}

double customFloor(double x) {
    if (x == (long int) x || x == INFINITY || x == -INFINITY) return x;
    return (x > 0) ? (long int) x : (long int) (x-1);
}

double customCeil(double x) {
    if (x == (long int) x || x == INFINITY || x == -INFINITY) return x;
    return (long int) (x+1);
}


long customDoubleFac(int n) {
    long result = 1;
    for (long i = n; i >= 0; i -= 2) {
        if (i == 0 || i == 1)
            return result;
        else
            result *= i;
    }
    return 0;
}
