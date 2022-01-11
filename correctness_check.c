#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <getopt.h>
#include "main.h"

double epsilon, a, b, step;
int precision;

void help(char *message) {
    if (message != NULL) fprintf(stderr, "%s", message);
    char *msg = "Bitte nutzen Sie folgendes Format\n"
                "./correctness_check <double> -a<double> -b<double> -s<double> -p<int>\n"
                "Der erste Wert ist Epsilon, und entspricht die gewünschte Genauigkeit\n"
                "[a,b] ist der Interval, in dem die Implementierung getestet wird\n"
                "-s gibt die Schrittweite\n"
                "-p gibt die gewünschte Präzision\n"
    fprintf(stderr,"%s", msg);
    exit(1);
}

void handle(int argc, char **argv) {
    if (argc != 6) help(NULL);
    char option;
    int a_flag = 0, b_flag = 0, s_flag = 0, p_flag = 0;
    char *str_err;
    while ((option = getopt(argc, argv, ":a:b:s:p:")) != -1) {
        switch (option) {
            case 'a':
                if (a_flag++ > 0) help(NULL);
                a = strtof(optarg, &str_err);
                if (*str_err != '\0') help(NULL);
                break;
            case 'b':
                if (b_flag++ > 0) help(NULL);
                b = strtof(optarg, &str_err);
                if (*str_err != '\0') help(NULL);
                break;
            case 's':
                if (s_flag++ > 0) help(NULL);
                step = strtof(optarg, &str_err);
                if (*str_err != '\0') help(NULL);
                break;
            case 'p':
                if (p_flag++ > 0) help(NULL);
                precision = strtol(optarg, &str_err, 10);
                if (*str_err != '\0') help(NULL);
                break;
            case ':':
                fprintf(stderr, "Argument für %c fehlt!\n", optopt);
                help(NULL);
                break;
            default:
                help(NULL);
        }
    }
    if (argc - optind == 1) {
        epsilon = strtof(argv[optind], &str_err);
        if (*str_err != '\0') help(NULL);
    } else help(NULL);
}


int main(int argc, char **argv) {
    handle(argc, argv);

    int sum = 0;
    printf("Running test for values in [%f, %f] with step size of %f...\n", a, b, step);
    for (double i = a; i <= b; i += step) {
        if ((approxArsinh_series(i, precision) - asinh(i)) > epsilon) sum++;
    }
    printf("Done.\nTotal test failed: %d\n", sum);

    exit(0);
}
