#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int i;        // loop counter
double x;     // x coordinate of needle's center
double k;     // angle between vertical position and needle
double l;     // length of the needle
double y;     // sin(angle) * l
double pi;    // output
long long n;  // amount of trials
long long p;  // positive trials

double get_cpu_time() { return (double)clock() / CLOCKS_PER_SEC; }

long long quick_pow10(int n) {
    static long long pow10[12] = {
        1,       10,       100,       1000,       10000,       100000,
        1000000, 10000000, 100000000, 1000000000, 10000000000, 100000000000};

    return pow10[n];
}

void lanzar() {
    for (i = 0; i < n; i++) {
        k = (double)rand() / (RAND_MAX)*2 * M_PI;  // random angle (0 - 2PI)
        x = (double)rand() / (RAND_MAX) / 2;       // random x (0 - 0.5)
        y = (l / 2) * sin(k);
        if (x <= y) {
            p++;
        }
    }
}

int main(int argc, char const *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Error. Digite el exponente de 10");
        return -1;
    }
    srand((int)clock());

    int exponent = atoi(argv[1]);
    if (exponent > 11) {
        fprintf(stderr, "Error: n es demasiado grande");
        return -1;
    }
    n = quick_pow10(exponent);
    p = 0;
    l = 1;

    // Comenzar a medir el tiempo
    double begin = get_cpu_time();

    // Dartboard algorithm
    lanzar();

    // Detener la medición del tiempo y calcular el tiempo transcurrido
    double end = get_cpu_time();
    double elapsed = (end - begin);
    printf("Time measured: %.3f seconds.\n", elapsed);

    double pi_approx = (l / p) * (n);
    double error = fabs(M_PI - pi_approx) / M_PI * 100;
    printf("Tosses: %lld, hits: %lld, pi_approx: %f, error: %f%%\n", n, p, pi_approx, error);

    // Escribir resultados en un archivo
    FILE *file = fopen("needles_seq.csv", "a");
    if (file == NULL) {
        printf("No se puede abrir needles_seq.csv");
        return -1;
    }
    fprintf(file, "%d, %lld, %lld, %f, %f, %f\n", exponent, n, p, pi_approx, error, elapsed);
    fclose(file);

    return 0;
}
