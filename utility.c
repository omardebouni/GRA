#define _POSIX_C_SOURCE 199309L

#include "utility.h"
#include "inverse_sinh.h"
#include "custom_math.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <getopt.h>
#include <math.h>
#include "alternatives.h"


/**
 * This function will run the version of the implementation
 * chosen by the user from the main method
 */
void run(double (*fn)(double), double x) {
    double result = (*fn)(x); // call the given function on the given x value
    printf("Arsinh(%lf) = %.20lf\n", x, result);
    printf("expected %.20lf\n", asinh(x));
}

/**
 * This function runs the chosen implementation while also measuring
 * the time it took to run the implementation, and the total time to run
 * the implementation for the number of times specified in the repetitions parameter.
 */
void runtime_analysis(double (*fn)(double), double x, long repetitions) {
    printf("Running a runtime analysis...\n");
    struct timespec startTotal;
    struct timespec endTotal;
    double timeTotal;

    double result = 0;
    clock_gettime(CLOCK_MONOTONIC, &startTotal); //Starting time clocking

    for (int i = 1; i <= repetitions; i++) {
        result = (*fn)(x);
    }
    clock_gettime(CLOCK_MONOTONIC, &endTotal); //Stop time clocking
    timeTotal = endTotal.tv_sec - startTotal.tv_sec + 1e-9 * (endTotal.tv_nsec - startTotal.tv_nsec);

    printf("Arsinh(%lf) = %.20lf\n", x, result);
    printf("expected %.20lf\n", asinh(x));;

    printf("Total time to run %ld %s: %.10lf seconds\n", repetitions, (repetitions > 1) ? "iterations" : "iteration", timeTotal);
}

/**
 * This function will take the user input given to the main method,
 * and will parse the arguments and options.
 * It will statically update/set the the values in the given variables
 */
void handle(int argc, char **argv, long *version, double *x, bool *analysis, long *repetitions) {
    if (argc < 2) print_help("No input was given. Please use the following format!\n");
    /* Saves the read option */
    char option;
    int v_flag = 0, b_flag = 0; // User can only specify each option once
    int testing = 0;
    char *str_err;

    while ((option = getopt(argc, argv, ":V:B::ht")) != -1 && !testing) {
        switch (option) {
            case 'V':
                if (v_flag++ > 0) print_help("The version can only be set once!\n"); //End program
                *version = strtol(optarg, &str_err, 10);
                if (*str_err != '\0') print_help("The specified version couldn't be parsed!\n");
                break;
            case 'B':
                if (b_flag++ > 0) print_help("The -B option can only be set once!\n"); //End program
                *analysis = true;
                if (optarg != NULL) {
                    *repetitions = strtol(optarg, &str_err, 10);
                    if (*str_err != '\0') print_help(NULL);
                }
                break;
            case 't':
                testing++;
                run_test();
                break;
            case ':':
                fprintf(stderr, "Argument f??r %c is missing!\n", optopt);
                print_help(NULL);
                break;
            default:
                print_help(NULL);
        }
    }
    if (argc - optind == 1) {
        *x = strtof(argv[optind], &str_err);
        if (*str_err != '\0')
            print_help("The given value couldn't be parsed. Please use the following format!\n");
    } else print_help(NULL);
}

/**
 * This function will output a helping message describing all options
 * and examples on how to call and run the program
 */
void print_help(char *message) {
    if (message != NULL) fprintf(stderr, "%s", message);
    char *help_msg = "Default: \n\t"
                     "Usage: ./arsinh <float>\n\n"
                     "Options:\n\t"
                     "-V<int>\t-- The implementation that should be used.\n\t"
                     "       \t   Possible inputs: V0-V5\n\t"
                     "       \t   If no version was given, default implementation will be used.\n\t"
                     "       \t   Version 0 is default, Version 1 uses a lookup table.\n\t"
                     "       \t   Versions 2-4 are old (worse) implementations. 5 is the one from <math.h>\n\n\t"
                     "-B<int>\t-- When set, a runtime analysis will be executed\n\t"
                     "       \t   and the measured time will be outputted. The argument of\n\t"
                     "       \t   this option is optional. It specifies the number of repetitions\n\n\t"
                     "-t     \t   When used, a testing program will run. The program will ask\n\t"
                     "       \t   user to enter some variables and a series of tests will be run\n\t"
                     "       \t   in a given interval with given step size. Finally test result\n\t"
                     "       \t   as well as total time to run wil be outputted\n\n\t"
                     "-h     \t-- Prints the help message that describes all the options\n\t"
                     "--help \t   of the program, and gives examples on how to use it.\n\n\t"
                     "Usage: ./main <float> -V<int> -B<int> \n\t\t    "
                     "./arsinh -h\n\n"
                     "Examples:\n\t"
                     "./arsinh 2.0\n\t"
                     "./arsinh -t\n\t"
                     "./arsinh -V0 -- -1.5 (For negative inputs a -- needs to be used before the input value)\n\t"
                     "./arsinh -V2 3.14159 -B\n\t"
                     "./arsinh 5 -B2\n";
    fprintf(stderr, "%s", help_msg);
    exit(1);
}

/* Auxiliary methods to read input from user */
void read_double(double *buf) {
    if (scanf("%lf", buf) == EOF) {
        fprintf(stderr, "Error while reading input!\n");
        exit(EXIT_FAILURE);
    }
}

void read_int(int *buf) {
    if (scanf("%d", buf) == EOF) {
        fprintf(stderr, "Error while reading input!\n");
        exit(EXIT_FAILURE);
    }
}

void read_str(char *buf) {
    if (scanf("%s", buf) == EOF) {
        fprintf(stderr, "Error while reading input!\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * Runs an interactive program to test the accuracy of the implementation
 * compared to the posix method asinh()
 */
void run_test() {
    if (system("clear") == -1)exit(-1); // just to silence warning
    printf("Running Analysis on the accuracy of the approximation function ...\n");
    double epsilon, a, b, step_size; //variables
    int end = 0, print, version; //flags
    int total, failed;
    char buf[10];
    double got, expected;
    /* A pointer to the implementation to be run according to the user */
    double (*fn)(double) = NULL;
    double (*compare)(double) = NULL; 
    while (!end) {
        printf("Choose the implementation you would like to test:\n\t");
        printf("0: The default version of the series_expansion implementation, (Most efficient/accurate)\n\t");
        printf("1: The lookup implementation\n\t");
        printf("2: The initial version of the series_expansion implementation (Very slow)\n\t");
        printf("3: The efficient and more accurate version of the series_expansion implementation\n\t");
        printf("4: Even more accurate version of the series_expansion implementation\n\t");
        printf("5: asinh() function from <math.h>\n\t");
        printf("6: test lookup_ln() against log() from <math.h>\n\t");
        printf("7: test customLn against log() from <math.h>\n\nEnter number: ");
        read_int(&version);
        printf("Enter the desired accuracy to compare with: ");
        read_double(&epsilon);
        printf("Enter the desired interval boundaries to be tested in starting with the left_bound: ");
        read_double(&a);
        printf("And the right_bound: ");
        read_double(&b);
        printf("Enter step size: ");
        read_double(&step_size);
        if (a > b || step_size == 0) {
            printf("Can't run tests for given arguments\n");
            return;
        }
        while (1) {
            printf("Print failed values? (yes/no)\n");
            read_str(buf);
            if (strcmp(buf, "yes") == 0) {
                print = 1;
                break;
            } else if (strcmp(buf, "no") == 0) {
                print = 0;
                break;
            }
        }

		compare = &asinh; //set back to default
        switch (version) {
            case 0:
                fn = &approxArsinh_series;
                break;
            case 1:
                fn = &approxArsinh_lookup;
                break;
            case 2:
                fn = &approxArsinh_series_V2;
                break;
            case 3:
                fn = &approxArsinh_series_V3;
                break;
            case 4:
                fn = &approxArsinh_series_V4;
                break;
            case 5:
                fn = &asinh;
                break;
            case 6:
                fn = &ln;
                compare = &log;
                break;
            case 7:
                fn = &customLn_V1;
                compare = &log;
                break;
            default:
                version = 0;
                fn = &approxArsinh_series;
                printf("The chosen version doesn't exist. Running the default version of the series_expansion implementation\n");
                break;
        }


        if (system("clear") == -1)exit(-1); // just to silence warning
        printf("Running test for values in [%lf, %lf]\nAccuracy: %lf\nStep size: %lf\nVersion: %d\n", a, b,epsilon, step_size, version);

        failed = 0;
        total = (b - a) * 1 / step_size + 1;

        struct timespec startTime;
        struct timespec endTime;
        double time;

        clock_gettime(CLOCK_MONOTONIC, &startTime); //Starting time clocking

        for (double i = a; i <= b; i += step_size) {
            got = (*fn)(i);
            expected = (*compare)(i);
            if ((got != got) || (customAbs(got) == INFINITY && expected != got) || (customAbs(got - expected) >= epsilon)) {
                failed++;
                if (print) printf("%s(%lf):\n\tGot:      %lf\n\tExpected: %lf\n",(version > 5)? "ln":"arsinh", i, got, expected);
                continue;
            }
        }

        clock_gettime(CLOCK_MONOTONIC, &endTime); //Stop time clocking
        time = endTime.tv_sec - startTime.tv_sec + 1e-9 * (endTime.tv_nsec - startTime.tv_nsec);
        printf("Done.\nTotal tests passed: %d/%d\n", (total - failed), total);
        printf("Total time to run all tests:  %f seconds\n", time);

        while (1) {
            printf("Run again? (yes/no)\n");
            read_str(buf);
            if (strcmp(buf, "yes") == 0) break;
            else if (strcmp(buf, "no") == 0) {
                end = 1;
                break;
            }
        }
    }
    printf("Analysis ended.\n");
    exit(0);
}
