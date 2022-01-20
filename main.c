#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "utility.h"
#include "inverse_sinh.h"


// TODO: Accept negative input values?

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
    long repetitions = 1; //default

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
            fn = &asinh;break;
        default:
            print_help("The chosen version is not available!\n");
    }

    /* This function will be run if the -B option was given
     * Defined in utility.c */
    if (analysis) runtime_analysis(fn, repetitions, x);
    else run(fn, x); // else the chosen implementation will just be called

    return 0;
}
