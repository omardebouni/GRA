#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include "custom_math.h"
#include "utility.h"

#define ITERATIONS 25



// TODO: Zeit Tracking
// TODO: Implementierung von lookup hinzufügen
// TODO: Change Makefile to produce arsinh to match Input Help Comments

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

/*
Create Lookuptable with Posix Function not optimized
*/
long double table[256];

void createPosixCompare(){
    for(int i = 0; i < 256; i++){
        table[i] = asinhl(2.0*3.1415927*(float)i/256.0);
    }
}

int main(int argc, char **argv) {
    /* The chosen version */
    long version = 0; // default
    /* Saves the inputted value to call the approximation function on */
    double x;
    /* When set, a runtime analysis will be called, and the measured times will be outputted */
    bool analysis = false;
    long repetitions = 1; //default

    /* This will handle all the user inputs/options and update the variables above */
    handle(argc, argv, &version, &x, &analysis, &repetitions);

    /* Used for Tracking Time in Functions*/
    struct timespec startTotal;
    struct timespec endTotal;
    struct timespec start;
    struct timespec end;
    double time;
    double timeTotal;


    double result = 0;
    clock_gettime(CLOCK_MONOTONIC, &startTotal); //Starting time clocking
    // the chosen version of implementation will be called
    switch (version) {
        case 0:
            clock_gettime(CLOCK_MONOTONIC, &start); //Starting time clocking
            result = approxArsinh_series(x);
            clock_gettime(CLOCK_MONOTONIC, &end); //Starting time clocking
            time = end.tv_sec - start.tv_sec + 1e-9 * (end.tv_nsec - start.tv_nsec);
            printf("Custom Arsinh has finished, time taken: %f\n", time);
            break;
        case 1:
            clock_gettime(CLOCK_MONOTONIC, &start); //Starting time clocking
            result = approxArsinh_lookup(x);
            clock_gettime(CLOCK_MONOTONIC, &end); //Starting time clocking
            time = end.tv_sec - start.tv_sec + 1e-9 * (end.tv_nsec - start.tv_nsec);
            printf("Arsinh_Lookup finished, time taken: %f\n", time);

            break;
        case 2:
            clock_gettime(CLOCK_MONOTONIC, &start); //Starting time clocking
            result = asinh(x);
            clock_gettime(CLOCK_MONOTONIC, &end); //Starting time clocking
            time = end.tv_sec - start.tv_sec + 1e-9 * (end.tv_nsec - start.tv_nsec);
            printf("Arsinh_Lookup finished, time taken: %f\n", time);
            break;
            //case ..
            //     ..
            //     ..
        default:
            print_help("The chosen version is not available!\n");
    }

    clock_gettime(CLOCK_MONOTONIC, &endTotal); //Stop time clocking
    timeTotal = endTotal.tv_sec - startTotal.tv_sec + 1e-9 * (endTotal.tv_nsec - startTotal.tv_nsec);
    printf("Programm has finished, total time taken:  %f\n", timeTotal);

    printf("Value of result = %f\n", result);
    printf("finished\n");
    return 0;
}
