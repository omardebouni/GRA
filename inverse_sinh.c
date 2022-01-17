#include "custom_math.h"

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

/*Die Funktion implementiert die Formel für ein bestimmtes x mithilfe eines Tabellen-
Lookups.*/
double approxArsinh_lookup(double x) {
    x += customSqrt(customPow(x, 2) + 1); // x + sqrt(x^2 + 1)
    return lookup_ln(x); // ln(x + sqrt(x^2 + 1))
}
