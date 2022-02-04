#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "utility.h"
#include "inverse_sinh.h"
#include "custom_math.h"
#include "alternatives.h"



/**
 * The main method will just start the program and call
 * the appropriate functions according to the given user
 * input from the command line.
 * The implementations of the are in the inverse_sinh.c file
 * Other functions can be found in utility.c
 */
int main(int argc, char **argv) {
    /* The chosen version */
    long version = 0; // default
    /* Saves the inputted value to call the approximation function on */
    double x;
    /* When set, a runtime analysis will be called, and the measured times will be outputted */
    bool analysis = false;
    long repetitions = 10000000; //default

    /* This will handle all the user inputs/options and update the variables above
     * Defined in utility.c */
    handle(argc, argv, &version, &x, &analysis, &repetitions);

    /* A pointer to the implementation to be run according to the user */
    double (*fn)(double) = NULL;

    switch (version) {
        case 0:
            fn = &approxArsinh_series;break;
        case 1:
            fn = &approxArsinh_lookup;break;
        case 2:
            fn = &approxArsinh_series_V2;break;
        case 3:
            fn = &approxArsinh_series_V3;break;
        case 4:
            fn = &approxArsinh_series_V4;break;
        case 5:
            fn = &asinh;break;
        default:
            print_help("The chosen version is not available!\n");
    }

    /* This function will be run if the -B option was given
     * Defined in utility.c */
    if (analysis) runtime_analysis(fn, x, repetitions);
    else run(fn, x); // else the chosen implementation will just be called

    return 0;
}
