#include "utility.h"
#include "inverse_sinh.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <getopt.h>
#include <math.h>


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
                fprintf(stderr, "Argument für %c is missing!\n", optopt);
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
                     "       \t   Possible inputs: V0-V3\n\t"
                     "       \t   If no version was given, version 0 will be used\n\t"
                     "       \t   as default. Version 1 is the one with a lookup table\n\n\t"
                     "-B<int>\t-- When set, a runtime analysis will be executed\n\t"
                     "       \t   and the measured time will be outputted. The argument of\n\t"
                     "       \t   this option is optional. It specifies the number of repetitions\n\n\t"
                     "-p<int>\t   This specifies the precision. A higher precision\n\t"
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

/* Helping methods to read input from user */
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
    printf("Running Analysis on the accuracy of the approximation function ...\n");
    double epsilon, a, b, step_size; //variables
    int end = 0, print; //flags
    int total, failed;
    char buf[10];
    double got, expected;
    while (!end) {
        printf("Enter the desired accuracy to compare with: ");
        read_double(&epsilon);
        printf("Enter the desired interval boundaries to be tested in starting with the left_bound: ");
        read_double(&a);
        printf("And the right_bound: ");
        read_double(&b);
        printf("Enter step size: ");
        read_double(&step_size);

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

        if (system("clear") == -1)exit(-1); // just to silence warning
        printf("Running test for values in [%lf, %lf]\nStep size: %lf\nAccuracy:%lf\n", a, b, step_size, epsilon);

        failed = 0;
        total = (b - a) * 1 / step_size;
        for (double i = a; i <= b; i += step_size) {
            got = approxArsinh_lookup(i);
            expected = asinh(i);
            if ((got - expected >= epsilon)) {
                failed++;
                if (print) printf("asinh(%lf):\n\tGot:      %lf\n\tExpected: %lf\n", i, got, expected);
            }
        }
        printf("Done.\nTotal tests passed: %d/%d\n", (total - failed), total);

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
