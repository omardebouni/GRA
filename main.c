#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <unistd.h>
#include <getopt.h>
#include "custom_math.h"
#include "main.h"

/* Variablen für die zu verwendende Version, und den übergebenen X Wert */
long version = 0, precision = 20;
double x;
/* Falls gesetzt, wird eine Performanz/Laufzeit Analyse ausgeführt, und die gemessene Zeiten ausgegeben */
bool analysis = false;
long iterations = 1; //default

void handle_args(int argc, char **argv);

void print_help(char *message);

// speichert die Werte für die Lookup-Tabelle  
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
    handle_args(argc, argv);
    double result = 0;
    // TODO: Verwende Preprocessor

    // die gewählte Version aufrufen
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
            print_help("Die gewählte Version ist nicht vorhanden!\n");
    }

    printf("Value of result = %f\n", result);
    printf("finished\n");
    return 0;
}

void handle_args(int argc, char **argv) {
    if (argc < 2) print_help("Keine Eingabe. Bitte nutzen Sie folgendes Format!\n");

    /* option speichert das char, das die Option spezifiziert */
    char option;
    int v_flag = 0, b_flag = 0, p_flag = 0; // User kann nur die Optionen je nur einmal benutzen
    char *str_err;

    while ((option = getopt(argc, argv, ":p:V:B::h")) != -1) {
        switch (option) {
            case 'V':
                if (v_flag++ > 0) print_help("Die Version darf nur einmal angegeben werden\n"); //Programm beenden
                version = strtol(optarg, &str_err, 10);
                if (*str_err != '\0') print_help("Die angegeben Version konnte nicht geparst werden\n");
                break;
            case 'B':
                if (b_flag++ > 0) print_help("Die -B Option darf nur einmal angegeben werden\n"); //Programm beenden
                analysis = true;
                if (optarg != NULL) {
                    iterations = strtol(optarg, &str_err, 10);
                    if (*str_err != '\0') print_help(NULL);
                }
                break;
            case 'p':
                if (p_flag++ > 0)
                    print_help("Die Precision-Option darf nur einmal angegeben werden\n"); //Programm beenden
                precision = strtol(optarg, &str_err, 10);
                if (*str_err != '\0') print_help(NULL);
                break;
            case ':':
                fprintf(stderr, "Argument für %c fehlt!\n", optopt);
                print_help(NULL);
                break;
            default:
                print_help(NULL);
        }
    }
    if (argc - optind == 1) {
        x = strtof(argv[optind], &str_err);
        if (*str_err != '\0')
            print_help("Der gegebene Wert konnte nicht geparst werden. Bitte nutzen Sie folgendes Format!\n");
    } else print_help(NULL);
}

void print_help(char *message) {
    if (message != NULL) fprintf(stderr, "%s", message);
    char *help_msg = "Default: \n\t"
                     "Verwendung: ./arsinh <float>\n\n"
                     "Optionen:\n\t"
                     "-V<int>\t-- Die Implementierung, die verwendet werden soll.\n\t"
                     "       \t   Mögliche Eingaben: V0-V3\n\n\t"
                     "-B<int>\t-- Falls gesetzt, wird die Laufzeit dazu\n\t"
                     "       \t   angemessen und ausgegeben. Das Argument  dieser\n\t"
                     "       \t   Option ist optional, und gibt die Anzahl an\n\t"
                     "       \t   Wiederholungen des Funktionsaufruf an.\n\n\t"
                     "-p<int>\t   Die gewünschte Precision. Eine höhere Precision\n\t"
                     "       \t   führt zu einer höheren Laufzeit, und genauerem \n\t"
                     "       \t   Ergebnis\n\n\t"
                     "-h     \t-- Gibt eine Beschreibung zur Verwendung des\n\t"
                     "--help \t   Programms und allen Optionen, bzw. Beispiele.\n\n\t"
                     "Verwendung: ./arsinh <float> -V<int> -B<int> -p<int>\n\t\t    "
                     "./arsinh -h\n\n"
                     "Beispielaufrufe:\n\t"
                     "./arsinh 2.0\n\t"
                     "./arsinh 2.0 -p20\n\t"
                     "./arsinh 1.5 -V0\n\t"
                     "./arsinh -V2 3.14159 -B -p10\n\t"
                     "./arsinh 5 -B2\n";
    fprintf(stderr, "%s", help_msg);
    exit(1);
}
