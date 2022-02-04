#include "custom_math.h"
#include "inverse_sinh.h"
#include <math.h>

double get_less1_weightOf(int n);
double get_larger1_weightOf(int n);

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

double get_less1_weightOf(int n) {
    static double weights_less1[] = {1.00000000000000000000000000000000000000000000000000,
                                     -0.16666666666666665741480812812369549646973609924316,
                                     0.07499999999999999722444243843710864894092082977295,
                                     -0.04464285714285714384841341484388976823538541793823,
                                     0.03038194444444444405895033867182064568623900413513,
                                     -0.02237215909090909185530371416916750604286789894104,
                                     0.01735276442307692387756468122006481280550360679626,
                                     -0.01396484375000000069388939039072283776476979255676,
                                     0.01155180089613970506601248189326724968850612640381,
                                     -0.00976160952919407839956100758627144386991858482361,
                                     0.00839033580961681506316107714837926323525607585907,
                                     -0.00731252587359884544809807138676660542842000722885,
                                     0.00644721031188964874975022567582527699414640665054,
                                     -0.00574003767084192359493322044272645143792033195496,
                                     0.00515330968231990458466862392583607288543134927750};
    return weights_less1[n];
}

double get_larger1_weightOf(int n) {
    static double weights_larger1[] = {0.25000000000000000000000000000000000000000000000000,
                                       -0.09375000000000000000000000000000000000000000000000,
                                       0.05208333333333333564629796796907612588256597518921,
                                       -0.03417968750000000000000000000000000000000000000000,
                                       0.02460937499999999930611060960927716223523020744324,
                                       -0.01879882812500000000000000000000000000000000000000,
                                       0.01496233258928571403789664628902755794115364551544,
                                       -0.01227378845214843750000000000000000000000000000000,
                                       0.01030392116970486188209932265635870862752199172974,
                                       -0.00880985260009765555611060960927716223523020744324,
                                       0.00764491341330788328617407145770812348928302526474,
                                       -0.00671584407488505074484974599613451573532074689865,
                                       0.00596080835048968995376617030501620320137590169907,
                                       -0.00533735645668847232675480185548622102942317724228,
                                       0.00481548160314559936523437500000000000000000000000};
    return weights_larger1[n];
}