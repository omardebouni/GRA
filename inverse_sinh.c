#include "custom_math.h"
#include "inverse_sinh.h"
#include <math.h>

#define M_E         2.71828182845904523536028747135266250
#define ITERATIONS 10

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
            temp += weights_less1[n] * customPow(x, 2 * n + 1); // loads ((-1)^n * (2n)!) / (2^(2n) * (n!)^2 * (2n+1))
            if (customAbs(temp) == INFINITY || temp != temp || temp == result) break; //if inf, -inf or NaN, or not changing anymore
            result = temp; // else save new value
            n++;
        }
    } else {
        // double fac of 33, so max n <= 16, but when terminating the sum after an even n the result is inaccurate, so max n == 15
        while (n++ < 15) {
            temp += weights_larger1[n-1] / customPow(absX, 2 * n); // loads (2n-1)!! / (2n * (2n)!!)
            if (temp == result || temp != temp || customAbs(temp) == INFINITY) break; //if inf, -inf or NaN. Cuts runtime by half
            result = temp;
        }
        result += lookup_ln(2 * absX);
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
    return lookup_ln(x); // ln(x + sqrt(x^2 + 1))
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
    return ln_table[index_of(closest_entry)] + lookup_ln(1 + customAbs(x - closest_entry) / closest_entry);
}





//5. Version
// 3decimal places
//Total tests passed: 19998939/20000001
//Total time to run all tests:  2.835520 seconds

//4. Version
//3decimal places
//Total tests passed: 19998875/20000001
//Total time to run all tests:  7.995230 seconds

//3. Version
//3decimal places
//Total tests passed: 19998019/20000001
//Total time to run all tests:  7.604277 seconds

//2. Version
//3decimal places
//Total tests passed: 19638840/20000001
//Total time to run all tests:  3.701794 seconds



// for 2 decimal places all versions passed all tests.
