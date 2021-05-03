#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double get_cpu_time() { return (double)clock() / CLOCKS_PER_SEC; }

long long quick_pow10(int n) {
    static long long pow10[12] = {1,       10,       100,       1000,       10000,       100000,
                                  1000000, 10000000, 100000000, 1000000000, 10000000000, 100000000000};
    // max 10^11
    return pow10[n];
}

int main(int argc, char const *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Error. Digite el exponente de 10");
        return -1;
    }
    // srand(time(NULL));
    srand((int)clock());
    const double factor = 1.0 / RAND_MAX;

    int exponent = atoi(argv[1]);
    long long n = quick_pow10(exponent);
    long long i, hits;
    double x, y;

    // Comenzar a medir el tiempo
    double begin = get_cpu_time();

    for (i = hits = 0; i < n; i++) {
        x = rand() * factor;
        y = rand() * factor;
        if (x * x + y * y < 1.0) hits++;
    }

    // Detener la medición del tiempo y calcular el tiempo transcurrido
    double end = get_cpu_time();
    double elapsed = (end - begin);
    printf("Time measured: %.3f seconds.\n", elapsed);

    printf("x: %f, y:%f\n", x, y);
    double pi_approx = 4.0 * hits / n;
    double error = fabs(M_PI - pi_approx) / M_PI * 100;
    printf("Approx. after %lld tosses: %f, error: %f%%\n", n, pi_approx, error);

    // Escribir resultados en un archivo
    // FILE *file = fopen("dartboard_seq.csv", "a");
    // if (file == NULL) {
    //     printf("No se puede abrir dartboard_seq.csv");
    //     return -1;
    // }
    // fprintf(file, "%d, %f\n", n, elapsed);
    // fclose(file);

    // while (1) {
    //     printf("no. of tosses\n");
    //     scanf("%d", &n);
    //     if (n <= 0) {
    //         break;
    //     }

    //     for (i = hits = 0; i < n; i++) {
    //         double x = rand() * factor;
    //         double y = rand() * factor;
    //         if (x * x + y * y < 1.0) hits++;
    //     }
    //     double pi_approx = 4.0 * hits / n;
    //     double error = fabs(M_PI - pi_approx) * 100 / M_PI;
    //     printf("Approx. after %d tosses: %f, error: %f%%\n", n, pi_approx, error);
    // }
    return 0;
}
