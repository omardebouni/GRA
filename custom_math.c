#include "custom_math.h"
#include <stdio.h>
#include <math.h> // Only to get -INFINITY without generating errors!

#define LOG_PRECISION  1000
#define LN_2 0.69314718055994528622676398299518041312694549560547


/**
 * This is the source file where all used mathematical functions are implemented.
 * Some functions are implemented more than once. Different implementations
 * differ in the efficiency and accuracy, and implement different formulas.
 * Some of the functions were implemented to work only for the specific cases
 * in which they will get called for.
 */


/**
 * This method implements a simple power function.
 * It expects a floating point value, raised to some integer exponent.
 * If the exponent is negative, it will calculate basis^exponent, and return
 * the inverse of the calculated value.
 */
double customPow(double basis, int exponent) {
    if (exponent == 0) return 1;
    if (basis == 0) {
        return (exponent < 0) ? INFINITY : 0; // 0^-x means division by 0, so will return inf
    }
    if (exponent == 1) return basis;

    int abs_exp = (exponent < 0) ? -exponent : exponent; // Take absolute value of exponent

    double result = basis;
    while (--abs_exp > 0 && result != INFINITY) {
        result *= basis;
    }
    return (exponent < 0) ? 1 / result : result;
}

/**
 * This method calculates the factorial function
 * Because we want to have the highst accuracy possible in the
 * approximation of sinh^-1, this function may return very large integers
 * hence it returns long long.
 */
long long customFac(int x) {
    if (x < 2) return 1;
    if (x > 19) return 0;
    long long res = x, i = x - 1;
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

/**
 * This is the first version of the natural logarithm implementation.
 * It implements the Maclaurin series for ln((x - 1) / (x + 1))
 * Explanation can be found here: https://socratic.org/questions/how-do-you-find-the-maclaurin-series-expansion-of-ln-1-x-1-x
 */
double customLn(double x) {
    if (x < 0) {
        fprintf(stderr, "customLn is undefined for negative values!\n");
        return -1; // this should signal an error to the calling function
    }
    if (x == 0) return -INFINITY;
    if (x == 1) return 0;
    double res = 1.0;
    double y = (x - 1) / (x + 1); // decompose x to x = (1+y)/(1-y). Solving for y yields y = (x-1)/(x+1)
    int even = 2, odd = 3;
    for (; even < LOG_PRECISION; even += 2, odd += 2) {
        res += (customPow(y, even) / odd); // 2y*(1/1 + 1/3 * y^2 + 1/5 * y^4 ....)
    }
    return 2 * y * res;
}

/* Babylonian method: https://www.geeksforgeeks.org/square-root-of-a-perfect-square/ */
double customSqrt(double n) {
    double x = n;
    double y = 1;
    double e = 0.000001; // precision level
    while (x - y > e) {
        x = (x + y) / 2;
        y = n / x;
    }
    return x;
}

double customFloor(double x) {
    /* According to the IEEE standard, x != x will be true iff f is NaN or -NaN*/
    if (x == (long int) x || customAbs(x) == INFINITY || x != x) return x;
    return (x > 0) ? (long int) x : (long int) (x - 1);
}


/**
 * This method implements the double factorial function.
 */
long long customDoubleFac(int n) {
    if (n > 33) {
        return 0;
    }
    if (n < 0) return 0;
    long result = 1;
    for (long i = n; i > 1; i -= 2) { //moves in steps of 2
        result *= i;
    }
    return result;
}

/**
 * Rounds values to the nearest whole integer, and
 * returns the result as a long.
 */
long roundToLong(double x) {
    return (x > 0) ? (long) (x + 0.5) : (long) -(-x + 0.5);
}

/**
 * This method implements the exponential function using Taylor expansion
 * around 0, which means that it provides a very good approximation for
 * values close to 0. To have an accurate approximation for all values,
 * the method exploits the properties of exponentiation to scale larger values down
 * to values very close to 0. It breaks the term e^x down to 2^k * e^r,
 * where x = k*ln(2) + r, k an integer value = round(x/ln(2)), and r is the rest.
 * Due to the properties of natural logarithm, the whole term breaks down to
 * 2^k * e^r. The rest of x - k*ln(2) = r is at max 0.5*ln(2). This is very efficient
 * because powers of 2 are very fast to calculate, and the taylor expansion needs only
 * a small number of iterations to come up with a good approximation of e^r
 */
double customExp(double x) {
    if (x != x) { // check for NaN
        printf("Received NaN in the customExp function! returning -1\n");
        return -1;
    }
    if (x == 0 || x == -0) return 1.0;
    double abs_x = customAbs(x); // Ignore the sign, calculate e^abs(x), and if x was negative return iverse

    long k = roundToLong(abs_x / LN_2);
    double rest = abs_x - k * LN_2; // calculate the rest

    //taylor for e^r
    double sum = 1 + rest; // the first 2 terms of the expansion
    int n = 1;

    while (n++ < 10) { // initial n to calculate with = 2
        sum += (customPow(rest, n) / customFac(n));
    }
    long pow_2_k = 1;
    pow_2_k <<= k; //efficiently calculate 2^k
    return (x > 0) ? (sum * pow_2_k) : 1 / (sum * pow_2_k);
}


/// TODO: explanation
/**
 * This method implements the natural logarithm function.
 * Because the first implementation wasn't efficient enough, this method was implemented.
 * It takes advantage of the cubic convergence of the Halley's method. It is extremely
 * more efficient than the first one. It scales the given value to bring it closer
 * to 1, because that is where the expansion is calculated, and where the results are most accurate.
 */
double customLn_V1(double x) {
    if (x < 0) {
        fprintf(stderr, "customLn_V1 is undefined for negative values!\n");
        return -1; // this should signal an error to the calling function
    }
    if (x == 0) return -INFINITY;
    if (x == 1) return 0;

    int factor = 0;
    while (x > 1.1) {
        x /= M_E;
        factor++;
    }

    while (x < 0.9) {
        x *= M_E;
        factor--;
    }

    double yn = x - 1.0; //y0 from taylor
    double yn_1 = yn + 2 * (x - customExp(yn)) / (x + customExp(yn));

    while (customAbs(yn - yn_1) > 0.0001) {
        yn = yn_1;
        yn_1 = yn + 2 * (x - customExp(yn)) / (x + customExp(yn));
    }
    return yn_1 + factor;
}

// calculates (2n)!/(n)!
long long halfFac(int n_2) {
    if (n_2 > 28) return 0;
    if (n_2 < 2) return 1;

    int n = n_2 / 2 + 1; // calculates n
    long long result = n;

    while (++n <= n_2) { // res = (n+1) * (n+2) * (n+3) * ... * (2n)
        result *= (n);
    }
    return result;
}

// efficient calculation of powers of 2
double customPow2(int n) {
    if (n == 0) return 1;
    long long result = 1;
    if (n > 0) result <<= n;

    return (double) result;
}

