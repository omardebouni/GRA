#include <stdbool.h>

void run_test();

void print_help(char *message);

void handle(int argc, char **argv, long *version, double *x, bool *analysis, long *repetitions);

void runtime_analysis(double (*fn)(double), double x, long repetitions);

void run(double (*fn)(double), double x);
