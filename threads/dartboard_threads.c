#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int exponent, num_threads;
long long n, tosses_per_thread, hits = 0;
pthread_mutex_t hits_mutex;

double get_cpu_time() { return (double)clock() / CLOCKS_PER_SEC; }

long long quick_pow10(int n) {
    static long long pow10[12] = {
        1,       10,       100,       1000,       10000,       100000,
        1000000, 10000000, 100000000, 1000000000, 10000000000, 100000000000};

    return pow10[n];
}

void *toss(void *arg) {
    const double factor = 1.0 / RAND_MAX;
    unsigned int seed = time(NULL);

    long long i, private_hits = 0;
    double x, y;
    // Darboard algorithm
    for (i = 0; i < tosses_per_thread; i++) {
        x = rand_r(&seed) * factor;
        y = rand_r(&seed) * factor;
        if (x * x + y * y < 1.0) private_hits++;
    }
    // Prevenir race condition
    pthread_mutex_lock(&hits_mutex);
    hits += private_hits;
    pthread_mutex_unlock(&hits_mutex);
    pthread_exit(NULL);
}

int main(int argc, char const *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Error. Digite el exponente de 10 y numero de hilos");
        return -1;
    }
    srand((int)clock());

    exponent = atoi(argv[1]);
    num_threads = atoi(argv[2]);
    if (exponent > 11) {
        fprintf(stderr, "Error: n es demasiado grande");
        return -1;
    }

    n = quick_pow10(exponent);
    if (n % num_threads != 0) {
        fprintf(stderr,
                "La división de 'n' entre el numero de hilos debe ser exacta");
        return -1;
    }
    tosses_per_thread = n / num_threads;

    // Crear los hilos
    pthread_t *threads = malloc(num_threads * sizeof(pthread_t));
    pthread_mutex_init(&hits_mutex, NULL);

    // Comenzar a medir el tiempo
    double begin = get_cpu_time();

    // Hacer el cálculo
    for (int i = 0; i < num_threads; i++)
        pthread_create(&threads[i], NULL, toss, NULL);

    // Unir los hilos
    for (int i = 0; i < num_threads; ++i) pthread_join(threads[i], NULL);

    // Detener la medición del tiempo y calcular el tiempo transcurrido
    double end = get_cpu_time();
    double elapsed = (end - begin);
    printf("Time measured: %.3f seconds.\n", elapsed);

    double pi_approx = 4.0 * hits / n;
    double error = fabs(M_PI - pi_approx) / M_PI * 100;
    printf("Tosses: %lld, hits: %lld, pi_approx: %f, error: %f%%\n", n, hits,
           pi_approx, error);

    // Escribir resultados en un archivo
    FILE *file = fopen("dartboard_threads.csv", "a");
    if (file == NULL) {
        printf("No se puede abrir dartboard_threads.csv");
        return -1;
    }
    fprintf(file, "%d, %d, %lld, %lld, %f, %f, %f\n", num_threads, exponent, n,
            hits, pi_approx, error, elapsed);
    fclose(file);

    pthread_mutex_destroy(&hits_mutex);
    pthread_exit(NULL);
    return 0;
}
