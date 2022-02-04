#include "alternatives.h"
#include "custom_math.h"
#include <math.h>
#include "lookup_tables.h"

/**
 * Here are some of the past versions of the inverse sinh function.
 * Each version differs in efficiency, i.e runtime, or accuracy.
 * All of the Versions below are callable from the main program, or through
 * the tester Program. To know what version has what number, and how to call them,
 * check the help instructions by running ./arsinh -h
 */


/**
 * This is the first version implemented. It is extremely inefficient,
 * and in general makes less accurate approximations than the other versions,
 * especially for larger (absolute) values. It implements the Taylor expansion formula.
 */
double approxArsinh_series_V2(double x) {
    if (x == 0) return 0;
    double result = 0;
    long long n = 0;
    if (x <= 1 && x >= -1) {
        while (n < 10) {
            result += (sign(n) * customFac(2 * n) * customPow(x, 2 * n + 1)) /
                      (customPow(2, 2 * n) * customPow(customFac(n), 2) * (2 * n + 1));
            n++;
        }
    } else {
        while (++n < 10) {
            result += (sign(n) * customFac(2 * n)) / (customPow(2, 2 * n) *
                                                      customPow(customFac(n), 2) * (2 * n) *
                                                      customPow(x, 2 * n));
        }
        if (x > 1) result = customLn_V1(2 * x) - result;
        else result = -customLn_V1(-2 * x) + result;
    }
    return result;
}

/**
 * This is the second implementation of the approximation using Taylor expansion.
 * For values between -1 and 1 it runs indefinitely instead of a fixed number of iterations,
 * and breaks when its no longer able to add more terms to the sum. For larger values keeping the
 * limit at 16 worked best. For both cases added a termination check, which approximately reduced runtime by half.
 * Also, it uses a new version of customLn, which is massively faster
 */
double approxArsinh_series_V3(double x) {
    if (x == 0) return 0;
    double result = 0, temp = 0, dividend, divisor;
    double absX = customAbs(x);
    int n = 0;
    if (absX <= 1) {
        while (1) {
            dividend = (sign(n) * customFac(2 * n) * customPow(x, 2 * n + 1));
            divisor = (customPow(2, 2 * n) * customPow(customFac(n), 2) * (2 * n + 1));
            if (divisor == 0) break;
            temp += dividend / divisor;
            if (customAbs(temp) == INFINITY || temp != temp || temp == result) break; //if inf, -inf or NaN
            result = temp; // else save value
            n++;
        }
    } else {
        while (n++ < 15) {
            dividend = sign(n - 1) * (customDoubleFac(2 * n - 1));
            divisor = (2 * n * customDoubleFac(2 * n) * customPow(absX, 2 * n));
            if (divisor == 0 || customAbs(divisor) == INFINITY || divisor != divisor)
                break; // check if factorial passed
            temp += dividend / divisor;
            if (temp == result) break; //if inf, -inf or NaN. Cuts runtime by half
            result = temp;
        }
        result += customLn_V1(2 * absX);
        if (x < 0) result *= -1;
    }
    return result;
}

/**
 * For this implementation, the term (2n)! was reduced by dividing it by one
 * of the two n! in the term (n!)^2 in the divisor. This allows the first case to sum to a maximum of 14
 * iterations, instead of just 10. Which led to better accuracy.
 */
double approxArsinh_series_V4(double x) {
    if (x == 0) return 0;
    double result = 0, temp = 0, dividend, divisor;
    double absX = customAbs(x);
    int n = 0;
    if (absX <= 1) {
        while (1) {
            dividend = (sign(n) * halfFac(2 * n) * customPow(x, 2 * n + 1));
            divisor = (customPow2(2*n) * customFac(n) * (2 * n + 1));
            if (divisor == 0 || customAbs(divisor) == INFINITY) break; // check if factorial passed
            temp += dividend / divisor;
            if (customAbs(temp) == INFINITY || temp != temp || temp == result) break; //if inf, -inf or NaN
            result = temp; // else save value
            n++;
        }
    } else {
        while (n++ < 15) {
            dividend = sign(n - 1) * (customDoubleFac(2 * n - 1));
            divisor = (2 * n * customDoubleFac(2 * n) * customPow(absX, 2 * n));
            if (divisor == 0 || customAbs(divisor) == INFINITY || divisor != divisor)
                break; // check if factorial passed
            temp += dividend / divisor;
            if (temp == result) break; //if inf, -inf or NaN. Cuts runtime by half
            result = temp;
        }
        result += customLn_V1(2 * absX);
        if (x < 0) result *= -1;
    }
    return result;
}

double approxArsinh_series_with_posix(double x) {
    if (x == 0) return 0;
    double result = 0, temp = 0;
    double absX = fabs(x);
    int n = 0;
    if (absX <= 1) {
        while (n < 15) { // fac(28) / fac(14)^2  is largest number computable, so the limit for n is < 15
            temp += get_less1_weightOf(n) * pow(x, 2 * n + 1); // loads ((-1)^n * (2n)!) / (2^(2n) * (n!)^2 * (2n+1))
            if (fabs(temp) == INFINITY || temp != temp || temp == result) break; //if inf, -inf or NaN, or not changing anymore
            result = temp; // else save new value
            n++;
        }
    } else {
        // double fac of 33, so max n <= 16, but when terminating the sum after an even n the result is inaccurate, so max n == 15
        while (n++ < 15) {
            temp += get_larger1_weightOf(n-1) / pow(absX, 2 * n); // loads (2n-1)!! / (2n * (2n)!!)
            if (temp == result || temp != temp || fabs(temp) == INFINITY) break; //if inf, -inf or NaN. Cuts runtime by half
            result = temp;
        }
        result += log(2 * absX);
        if (x < 0) result *= -1;
    }
    return result;
}
