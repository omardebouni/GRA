#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
//#include <iostream>
#include <getopt.h>
#include "custom_math.h"

void print_help(char *message);

void run_test();

// TODO: Bestimme einen besserten Wert statt 10000
// TODO: Verwende Preprocessor
double lookup_values[10000];

/*Die Funktion implementiert die Formel für ein bestimmtes x mithilfe einer reinen
Reihendarstellung und gibt das Ergebnis zurück
https://proofwiki.org/wiki/Power_Series_Expansion_for_Real_Area_Hyperbolic_Sine */
double approxArsinh_series(double x, int precision) {
    double absX = customAbs(x);
    double result = 0;
    double dividend, divisor;
    if (absX <= 1) {
        for (int n = 0; n < precision; n++) {
            dividend = sign(n) * customFactorial(2 * n) * customPow(x, 2 * n + 1);
            divisor = customPow(2, 2 * n) * customPow(customFactorial(n), 2) * (2 * n + 1);
            result += (dividend / divisor);
        }
    } else if (x >= 1) {
        for (int n = 1; n < precision; n++) {
            dividend = sign(n) * customFactorial(2 * n);
            divisor = customPow(2, 2 * n) * customPow(customFactorial(n), 2) * (2 * n) * customPow(x, 2 * n);
            result += (dividend / divisor);
        }
        result = customLn(2 * x) - result;
    } else if (x <= -1) {
        for (int n = 0; n < precision; n++) {
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
double approxArsinh_lookup(double x, int precision) {
    double absX = customAbs(x);
    double result = 0;
    double dividend, divisor;
    if (absX <= 1) {
        for (int n = 0; n < precision; n++) {
            dividend = lookup_values[n] * customPow(x, 2 * n + 1);
            divisor = (2 * n + 1);
            result += (dividend / divisor);
        }
    } else if (x >= 1) {
        for (int n = 1; n < precision; n++) {
            dividend = lookup_values[n];
            divisor = (2 * n) * customPow(x, 2 * n);
            result += (dividend / divisor);
        }
        result = customLn(2 * x) - result;
    } else if (x <= -1) {
        for (int n = 0; n < precision; n++) {
            dividend = lookup_values[n];
            divisor = (2 * n) * customPow(x, 2 * n);
            if (divisor != 0) {
                result += (dividend / divisor);
            }
        }
        result = -customLn(-2 * x) + result;
    }
    return result;
}


int main(int argc, char **argv) {
    /* Variables for the chosen version and precision */
    long version = 0, precision = 20; //default
    double x;
    /* When set, a runtime analysis will run, and the measured times will be outputted */
    bool analysis = false;
    long iterations = 1; //default


    if (argc < 2) print_help("No input was given. Please use the following format!\n");
    /* Saves the read option */
    char option;
    int v_flag = 0, b_flag = 0, p_flag = 0; // User can only specify each option once
    int testing = 0;
    char *str_err;

    while ((option = getopt(argc, argv, ":p:V:B::ht")) != -1 && !testing) {
        switch (option) {
            case 'V':
                if (v_flag++ > 0) print_help("The version can only be set once!\n"); //End program
                version = strtol(optarg, &str_err, 10);
                if (*str_err != '\0') print_help("The specified version couldn't be parsed!\n");
                break;
            case 'B':
                if (b_flag++ > 0) print_help("The -B option can only be set once!\n"); //End program
                analysis = true;
                if (optarg != NULL) {
                    iterations = strtol(optarg, &str_err, 10);
                    if (*str_err != '\0') print_help(NULL);
                }
                break;
            case 'p':
                if (p_flag++ > 0)
                    print_help("The precession can only be specified once!\n");
                precision = strtol(optarg, &str_err, 10);
                if (*str_err != '\0') print_help(NULL);
                break;
            case 't':
                testing++;
                run_test();
                break;
            case ':':
                fprintf(stderr, "Argument für %c is missing!\n", optopt);
                print_help(NULL);
                break;
            default:
                print_help(NULL);
        }
    }
    if (argc - optind == 1) {
        x = strtof(argv[optind], &str_err);
        if (*str_err != '\0')
            print_help("The given value couldn't be parsed. Please use the following format!\n");
    } else print_help(NULL);


    double result = 0;

    // the chosen version of implementation will be called
    switch (version) {
        case 0:
            result = approxArsinh_series(x, precision);
            break;
        case 1:
            initializeLookupArray(lookup_values, precision);
            result = approxArsinh_lookup(x, precision);
            break;
        case 2:
            break;
            //case ..
            //     ..
            //     ..
        default:
            print_help("The chosen version is not available!\n");
    }

    printf("Value of result = %f\n", result);
    printf("finished\n");
    return 0;
}


void print_help(char *message) {
    if (message != NULL) fprintf(stderr, "%s", message);
    char *help_msg = "Default: \n\t"
                     "Usage: ./arsinh <float>\n\n"
                     "Options:\n\t"
                     "-V<int>\t-- The implementation that should be used.\n\t"
                     "       \t   Possible inputs: V0-V3\n\t"
                     "       \t   If no version was given, version 0 will be used\n\t"
                     "       \t   as default. Version 1 is the one with a lookup table\n\n\t"
                     "-B<int>\t-- When set, a runtime analysis will be executed\n\t"
                     "       \t   and the measured time will be outputted. The argument of\n\t"
                     "       \t   this option is optional. It specifies the number of repetitions\n\n\t"
                     "-p<int>\t   This specifies the precession. A higher precession\n\t"
                     "       \t   leads to a longer runtime, and a more accurate result\n\n\t"
                     "-h     \t-- Prints the help message that describes all the options\n\t"
                     "--help \t   of the program, and gives examples on how to use it.\n\n\t"
                     "Usage: ./arsinh <float> -V<int> -B<int> -p<int>\n\t\t    "
                     "./arsinh -h\n\n"
                     "Examples:\n\t"
                     "./arsinh 2.0\n\t"
                     "./arsinh 2.0 -p20\n\t"
                     "./arsinh 1.5 -V0\n\t"
                     "./arsinh -V2 3.14159 -B -p10\n\t"
                     "./arsinh 5 -B2\n";
    fprintf(stderr, "%s", help_msg);
    exit(1);
}

void run_test() {
    printf("Running Analysis on the accuracy of the approximation function ...\n");
    double epsilon, a, b, step_size;
    int end = 0;
    char buf[10];
    while (!end) {
        printf("Enter the desired precession to compare with: ");
        scanf("%lf", &epsilon);
        printf("Enter the desired interval boundaries to be tested in starting with the left_bound: ");
        scanf("%lf", &a);
        printf("And the right_bound: ");
        scanf("%lf", &b);
        printf("Enter step size: ");
        scanf("%lf", &step_size);
        system("clear");
        printf("Running test for values in [%f, %f] with step size of %f...\n", a, b, step_size);

        int sum = 0;
        for (double i = a; i <= b; i += step_size) {
            if ((approxArsinh_series(i, 20) - asinh(i) > epsilon)) sum++;
        }
        printf("Done.\nTotal test failed: %d\n", sum);
        while (1) {
            printf("Run again? (yes/no)\n");
            scanf("%s", buf);
            if (strcmp(buf, "yes") == 0) break;
            else if (strcmp(buf, "no") == 0) {
                end = 1;
                break;
            }
        }

    }

    exit(0);
}

