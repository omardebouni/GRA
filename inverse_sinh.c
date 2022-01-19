#include "custom_math.h"
#include "inverse_sinh.h"
#include <math.h>

#define ITERATIONS 25


/*Die Funktion implementiert die Formel für ein bestimmtes x mithilfe einer reinen
Reihendarstellung und gibt das Ergebnis zurück
https://proofwiki.org/wiki/Power_Series_Expansion_for_Real_Area_Hyperbolic_Sine */
double approxArsinh_series(double x) {
    double absX = customAbs(x);
    double result = 0;
    double dividend, divisor;
    if (absX <= 1) {
        for (int n = 0; n < ITERATIONS; n++) {
            dividend = sign(n) * customFactorial(2 * n) * customPow(x, 2 * n + 1);
            divisor = customPow(2, 2 * n) * customPow(customFactorial(n), 2) * (2 * n + 1);
            result += (dividend / divisor);
        }
    } else if (x >= 1) {
        for (int n = 1; n < ITERATIONS; n++) {
            dividend = sign(n) * customFactorial(2 * n);
            divisor = customPow(2, 2 * n) * customPow(customFactorial(n), 2) * (2 * n) * customPow(x, 2 * n);
            result += (dividend / divisor);
        }
        result = customLn(2 * x) - result;
    } else if (x <= -1) {
        for (int n = 0; n < ITERATIONS; n++) {
            dividend = customPow(-1, n) * customFactorial(2 * n);
            divisor = customPow(2, 2 * n) * customPow(customFactorial(n), 2) * (2 * n) * customPow(x, 2 * n);
            if (divisor != 0) {
                result += (dividend / divisor);
            }
        }
        result = -customLn(-2 * x) + result;
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
    return (int) customFloor((x - 1) * 100);
}

/**
 * This is an auxiliary function for the lookup function.
 * It returns the logarithm of the closest value to the
 * given x parameter. The values returned are between 1.00 and 9.99
 * with a step size of 0.01
 */
double get_closest_entry(double x) {
    x *= 100;
    double floored = customFloor(x);
    if ((x - floored) < 0.5) return floored / 100;
    else return (floored + 1) / 100;
}

// TODO: implement ln using a lookup table
double lookup_ln(double x) {
    if (x == 0) return -INFINITY;
    if (x < 1) return -1 + lookup_ln(x * M_E);
    else if (x > 10) return 1 + lookup_ln(x / M_E);

    double closest_entry = get_closest_entry(x);
    return ln_table[index_of(closest_entry)] + lookup_ln(1 + customAbs(x - closest_entry));
}

/*Die Funktion implementiert die Formel für ein bestimmtes x mithilfe eines Tabellen-
Lookups.*/
double approxArsinh_lookup(double x) {
    x += customSqrt(customPow(x, 2) + 1); // x + sqrt(x^2 + 1)
    return lookup_ln(x); // ln(x + sqrt(x^2 + 1))
}
