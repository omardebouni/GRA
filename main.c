#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include "utility.h"
#include "inverse_sinh.h"



// TODO: Zeit Tracking
// TODO: Implementierung von lookup hinzufügen
// TODO: Change Makefile to produce arsinh to match Input Help Comments



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
    
    printf("version is %l, x is %lf, anal %d, rep is %l\n",version, x, analysis, repetitions);

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
