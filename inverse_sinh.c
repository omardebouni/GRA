#include "custom_math.h"
#include "inverse_sinh.h"
#include <math.h>
#include <stdio.h>

#define M_E         2.71828182845904523536028747135266250
#define ITERATIONS 10



/*Die Funktion implementiert die Formel für ein bestimmtes x mithilfe einer reinen
Reihendarstellung und gibt das Ergebnis zurück
https://proofwiki.org/wiki/Power_Series_Expansion_for_Real_Area_Hyperbolic_Sine */
double approxArsinh_series(double x) {
    double result = 0;
    long long n = 0;
    if (x <=1 && x >= -1) {
        while (n < ITERATIONS) {
            result += (sign(n) * customFactorial(2 * n) * customPow(x, 2 * n + 1)) /
                    (customPow(2, 2 * n) * customPow(customFactorial(n), 2) * (2 * n + 1));
            n++;
        }
    } else {
        while (++n < ITERATIONS) {
            result += (sign(n) * customFactorial(2 * n)) / (customPow(2, 2 * n) *
                    customPow(customFactorial(n), 2) * (2 * n) * customPow(x, 2*n));
        }
        if (x > 1) result = customLn(2 * x) - result;
        else result = -customLn(-2 * x) + result;
    }
    return result;
}
/**
 * This is an auxiliary function for the lookup function.
 * It expects only values of which the natural logarithm
 * is precalculated and saved in the lookup table array (between 1.00 and 9.99).
 * Returns the index of the sought logarithm as an int
 */
int index_of(double x) {
    return (int) customFloor((x - 1) * 1000);
}

/**
 * This is an auxiliary function for the lookup function.
 * It returns the logarithm of the closest value to the
 * given x parameter. The values returned are between 1.00 and 9.99
 * with a step size of 0.01
 */
double get_closest_entry(double x) {
    x *= 1000;
    double floored = customFloor(x);
    if ((x - floored) < 0.5) return floored / 1000;
    else return (floored + 1) / 1000;
}

/**
 * This function will approximate the natural logarithm using a lookup table.
 * The lookup table has the logarithms of values between 1.00 and 9.99 with a
 * step size of 0.01. This function then recursively scales all given values by multiplying/
 * dividing them by e and adding/subtracting ln(e) = 1 each time, using the multiplication
 * properties of logarithms (ln(c * x) = ln(c) + ln(x)).
 * In the end the function will receive an x value between 1 and 10.
 * To calculate the logarithm of such values, this function takes advantage of the Lookup table
 * It will separate the received value x into to values, x = y + r
 * where y is one of the values of which the logarithm is already calculated in the table
 * Then following the formula ln(y + r) = ln(y * (1 + r/y)) = ln(y) + ln(1 + r/y),
 * the function will return ln(y) + ln(1 + r/y). The recursion ends when r/y gets smaller than
 * step size of 0.01, then the closest value will be returned from the table
 */
double lookup_ln(double x) {
    if (x == 0) return -INFINITY;
    if (x < 1) return -1 + lookup_ln(x * M_E);
    else if (x > 10) return 1 + lookup_ln(x / M_E);

    //recursion base
    if (x <= 1.001) return ln_table[index_of(get_closest_entry(x))];

    double closest_entry = get_closest_entry(x);
    return ln_table[index_of(closest_entry)] + lookup_ln(1 + customAbs(x - closest_entry)/closest_entry);
}


/**
 * This function approximates the inverse hyperbolic sine using the formula
 * arsinh(x) = ln(x + sqrt(x^2 + 1)), and the function lookup_ln, which uses a
 * lookup table to approximate the natural logarithm.
 */
double approxArsinh_lookup(double x) {
    x += customSqrt(customPow(x, 2) + 1); // x + sqrt(x^2 + 1)
    return lookup_ln(x); // ln(x + sqrt(x^2 + 1))
}

double approxArsinh_series_V1(double x) {
    double result = 0;
    int n = 0;
    double oben, unten;
    while (n++ < 10) {
        oben = customDoubleFac(2*n-1);
        unten = 2*n * customDoubleFac(2*n) * customPow(x,2*n);
        result += sign(n-1) * oben/unten;
    }
    return result + customLn(2*x);
}
