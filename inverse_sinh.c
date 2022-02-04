#include "custom_math.h"
#include "inverse_sinh.h"
#include <math.h>
#include "lookup_tables.h"

/**
 * This method approximates the inverse hyperbolic sine using 2 different series
 * expansions. For values between -1 and 1 the Taylor expansion about 1 is used. For larger values
 * it uses an expansion around infinity with a high convergence rate. It takes use of the two tables
 * in which the factorial/double factorial values from the formula as well as the sign and other variables,
 * are precalculated.
 */
double approxArsinh_series(double x) {
    if (x == 0) return 0;
    double result = 0, temp = 0;
    double absX = customAbs(x);
    int n = 0;
    if (absX <= 1) {
        while (n < 15) { // fac(28) / fac(14)^2  is largest number computable, so the limit for n is < 15
            temp += get_less1_weightOf(n) * customPow(x, 2 * n + 1); // loads ((-1)^n * (2n)!) / (2^(2n) * (n!)^2 * (2n+1))
            if (customAbs(temp) == INFINITY || temp != temp || temp == result) break; //if inf, -inf or NaN, or not changing anymore
            result = temp; // else save new value
            n++;
        }
    } else {
        // double fac of 33, so max n <= 16, but when terminating the sum after an even n the result is inaccurate, so max n == 15
        while (n++ < 15) {
            temp += get_larger1_weightOf(n-1) / customPow(absX, 2 * n); // loads (2n-1)!! / (2n * (2n)!!)
            if (temp == result || temp != temp || customAbs(temp) == INFINITY) break; //if inf, -inf or NaN. Cuts runtime by half
            result = temp;
        }
        result += ln(2 * absX);
        if (x < 0) result *= -1;
    }
    return result;
}


/**
 * This function approximates the inverse hyperbolic sine using the formula
 * arsinh(x) = ln(x + sqrt(x^2 + 1)), and the function lookup_ln, which uses a
 * lookup table to approximate the natural logarithm.
 */
double approxArsinh_lookup(double x) {
    if (x == 0) return 0;
    x += customSqrt(customPow(x, 2) + 1); // x + sqrt(x^2 + 1)
    return ln(x); // ln(x + sqrt(x^2 + 1))
}

