#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double get_cpu_time() { return (double)clock() / CLOCKS_PER_SEC; }

long long quick_pow10(int n) {
    static long long pow10[12] = {
        1,       10,       100,       1000,       10000,       100000,
        1000000, 10000000, 100000000, 1000000000, 10000000000, 100000000000};

    return pow10[n];
}

int main(int argc, char const *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Error. Digite el exponente de 10");
        return -1;
    }
    // srand((int)clock());
    const double factor = 1.0 / RAND_MAX;

    int exponent = atoi(argv[1]);
    if (exponent > 11) {
        fprintf(stderr, "Error: n es demasiado grande");
        return -1;
    }
    long long n = quick_pow10(exponent);
    long long hits = 0;
    double x, y;

    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    // Comenzar a medir el tiempo
    double begin = get_cpu_time();
    double wall_start = MPI_Wtime();

    // Get the number of processes and rank
    int num_processes, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Obtener el numero de lanzamientos por proceso
    long long tosses = n / num_processes;

    // Transmitir el valor anterior a todos los procesos
    MPI_Bcast(&tosses, 1, MPI_INT, 0, MPI_COMM_WORLD);

    unsigned int seed = time(NULL);
    long long local_hits = 0;
    // Dartboard algorithm
    for (int i = 0; i < tosses; i++) {
        x = rand_r(&seed) * factor;
        y = rand_r(&seed) * factor;
        if (x * x + y * y < 1.0) local_hits++;
    }

    // Hacer una reducción de suma con los hits de cada proceso
    MPI_Reduce(&local_hits, &hits, 1, MPI_LONG_LONG_INT, MPI_SUM, 0,
               MPI_COMM_WORLD);

    if (rank == 0) {
        double wall_end = MPI_Wtime();
        double wall_time = wall_end - wall_start;
        // Detener la medición del tiempo y calcular el tiempo transcurrido
        double end = get_cpu_time();
        double elapsed = (end - begin);
        printf("Time measured: %.3f seconds.\n", elapsed);
        printf("WTime measured: %.3f seconds.\n", wall_time);

        double pi_approx = 4.0 * hits / n;
        double error = fabs(M_PI - pi_approx) / M_PI * 100;
        printf("Tosses: %lld, hits: %lld, pi_approx: %f, error: %f%%\n", n, hits, pi_approx, error);

        // Escribir resultados en un archivo
        FILE *file = fopen("dartboard_mpi.csv", "a");
        if (file == NULL) {
            printf("No se puede abrir dartboard_mpi.csv");
            return -1;
        }
        fprintf(file, "%d, %d, %lld, %lld, %f, %f, %f, %f\n",
                num_processes, exponent, n, hits, pi_approx, error, elapsed, wall_time);
        fclose(file);
    }

    // Finalize the MPI environment
    MPI_Finalize();
    return 0;
}
