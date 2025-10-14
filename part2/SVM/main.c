#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#ifndef PARALLEL
#define PARALLEL 0
#endif

float set_vers_svs[16][2] = {
    {1.9, 0.2},
    {3.3, 1.0},
};
float set_vers_alphas[2] = {
    0.8,
    -0.8,
};
float set_vers_bias = 3.169231;
float set_virg_svs[16][2] = {
    {1.9, 0.2},
    {4.5, 1.7},
};
float set_virg_alphas[2] = {
    0.2,
    -0.2,
};
float set_virg_bias = 2.163152;
float versi_virg_svs[16][2] = {
    {4.7, 1.4},
    {4.5, 1.5},
    {4.9, 1.5},
    {4.6, 1.5},
    {4.7, 1.6},
    {4.7, 1.4},
    {4.8, 1.8},
    {4.9, 1.5},
    {5.1, 1.9},
    {4.5, 1.7},
    {5.1, 2.0},
    {5.3, 1.9},
    {5.0, 2.0},
    {5.0, 1.5},
    {4.9, 2.0},
    {4.9, 1.8}};
float versi_virg_alphas[16] = {
    1.0,
    0.2,
    1.0,
    1.0,
    1.0,
    1.0,
    1.0,
    1.0,
    -1.0,
    -1.0,
    -1.0,
    -0.2,
    -1.0,
    -1.0,
    -1.0,
    -1.0,
};
float versi_virg_bias = 10.54;

float svm_compute(float sample[], int n_svs, float svs[][2], float alphas[], float bias)
{

    int i = 0;
    float acc_sum = 0;

    for (i = 0; i < n_svs; i++)
    {

        acc_sum += ((sample[0] * svs[i][0] + sample[1] * svs[i][1]) * alphas[i]);
    }

    return acc_sum + bias;
}

typedef enum
{
    SET = 0,
    VERS,
    VIRG
} final_classes_t;

final_classes_t classify(float vals[])
{

    int flower[3] = {0, 0, 0};

    if (vals[0] > 0)
    {
        flower[0] += 1;
    }
    else
    {
        flower[1] += 1;
    }

    if (vals[1] > 0)
    {
        flower[0] += 1;
    }
    else
    {
        flower[2] += 1;
    }
    if (vals[2] > 0)
    {
        flower[1] += 1;
    }
    else
    {
        flower[2] += 1;
    }

    int i;
    int final_class = 0;
    int vote_count = flower[0];
    for (i = 1; i < 3; i++)
    {

        if (flower[i] > vote_count)
        {
            vote_count = flower[i];
            final_class = i;
        }
    }

    return final_class;
}

int main(int argc, char *argv[])
{
    // Valores padrão
    int threads = 1;
    int num_reps = 100;

    // Ler argumentos da linha de comando
    if (argc >= 2)
        num_reps = atoi(argv[1]);
    if (argc >= 3)
        threads = atoi(argv[2]);

    float samples[150][3] = {
        {1.4, 0.2, 0.0},
        {1.4, 0.2, 1.0},
        {1.3, 0.2, 2.0},
        {1.5, 0.2, 3.0},
        {1.4, 0.2, 4.0},
        {1.7, 0.4, 5.0},
        {1.4, 0.3, 6.0},
        {1.5, 0.2, 7.0},
        {1.4, 0.2, 8.0},
        {1.5, 0.1, 9.0},
        {1.5, 0.2, 10.0},
        {1.6, 0.2, 11.0},
        {1.4, 0.1, 12.0},
        {1.1, 0.1, 13.0},
        {1.2, 0.2, 14.0},
        {1.5, 0.4, 15.0},
        {1.3, 0.4, 16.0},
        {1.4, 0.3, 17.0},
        {1.7, 0.3, 18.0},
        {1.5, 0.3, 19.0},
        {1.7, 0.2, 20.0},
        {1.5, 0.4, 21.0},
        {1.0, 0.2, 22.0},
        {1.7, 0.5, 23.0},
        {1.9, 0.2, 24.0},
        {1.6, 0.2, 25.0},
        {1.6, 0.4, 26.0},
        {1.5, 0.2, 27.0},
        {1.4, 0.2, 28.0},
        {1.6, 0.2, 29.0},
        {1.6, 0.2, 30.0},
        {1.5, 0.4, 31.0},
        {1.5, 0.1, 32.0},
        {1.4, 0.2, 33.0},
        {1.5, 0.1, 34.0},
        {1.2, 0.2, 35.0},
        {1.3, 0.2, 36.0},
        {1.5, 0.1, 37.0},
        {1.3, 0.2, 38.0},
        {1.5, 0.2, 39.0},
        {1.3, 0.3, 40.0},
        {1.3, 0.3, 41.0},
        {1.3, 0.2, 42.0},
        {1.6, 0.6, 43.0},
        {1.9, 0.4, 44.0},
        {1.4, 0.3, 45.0},
        {1.6, 0.2, 46.0},
        {1.4, 0.2, 47.0},
        {1.5, 0.2, 48.0},
        {1.4, 0.2, 49.0},
        {4.7, 1.4, 50.0},
        {4.5, 1.5, 51.0},
        {4.9, 1.5, 52.0},
        {4.0, 1.3, 53.0},
        {4.6, 1.5, 54.0},
        {4.5, 1.3, 55.0},
        {4.7, 1.6, 56.0},
        {3.3, 1.0, 57.0},
        {4.6, 1.3, 58.0},
        {3.9, 1.4, 59.0},
        {3.5, 1.0, 60.0},
        {4.2, 1.5, 61.0},
        {4.0, 1.0, 62.0},
        {4.7, 1.4, 63.0},
        {3.6, 1.3, 64.0},
        {4.4, 1.4, 65.0},
        {4.5, 1.5, 66.0},
        {4.1, 1.0, 67.0},
        {4.5, 1.5, 68.0},
        {3.9, 1.1, 69.0},
        {4.8, 1.8, 70.0},
        {4.0, 1.3, 71.0},
        {4.9, 1.5, 72.0},
        {4.7, 1.2, 73.0},
        {4.3, 1.3, 74.0},
        {4.4, 1.4, 75.0},
        {4.8, 1.4, 76.0},
        {5.0, 1.7, 77.0},
        {4.5, 1.5, 78.0},
        {3.5, 1.0, 79.0},
        {3.8, 1.1, 80.0},
        {3.7, 1.0, 81.0},
        {3.9, 1.2, 82.0},
        {5.1, 1.6, 83.0},
        {4.5, 1.5, 84.0},
        {4.5, 1.6, 85.0},
        {4.7, 1.5, 86.0},
        {4.4, 1.3, 87.0},
        {4.1, 1.3, 88.0},
        {4.0, 1.3, 89.0},
        {4.4, 1.2, 90.0},
        {4.6, 1.4, 91.0},
        {4.0, 1.2, 92.0},
        {3.3, 1.0, 93.0},
        {4.2, 1.3, 94.0},
        {4.2, 1.2, 95.0},
        {4.2, 1.3, 96.0},
        {4.3, 1.3, 97.0},
        {3.0, 1.1, 98.0},
        {4.1, 1.3, 99.0},
        {6.0, 2.5, 100.0},
        {5.1, 1.9, 101.0},
        {5.9, 2.1, 102.0},
        {5.6, 1.8, 103.0},
        {5.8, 2.2, 104.0},
        {6.6, 2.1, 105.0},
        {4.5, 1.7, 106.0},
        {6.3, 1.8, 107.0},
        {5.8, 1.8, 108.0},
        {6.1, 2.5, 109.0},
        {5.1, 2.0, 110.0},
        {5.3, 1.9, 111.0},
        {5.5, 2.1, 112.0},
        {5.0, 2.0, 113.0},
        {5.1, 2.4, 114.0},
        {5.3, 2.3, 115.0},
        {5.5, 1.8, 116.0},
        {6.7, 2.2, 117.0},
        {6.9, 2.3, 118.0},
        {5.0, 1.5, 119.0},
        {5.7, 2.3, 120.0},
        {4.9, 2.0, 121.0},
        {6.7, 2.0, 122.0},
        {4.9, 1.8, 123.0},
        {5.7, 2.1, 124.0},
        {6.0, 1.8, 125.0},
        {4.8, 1.8, 126.0},
        {4.9, 1.8, 127.0},
        {5.6, 2.1, 128.0},
        {5.8, 1.6, 129.0},
        {6.1, 1.9, 130.0},
        {6.4, 2.0, 131.0},
        {5.6, 2.2, 132.0},
        {5.1, 1.5, 133.0},
        {5.6, 1.4, 134.0},
        {6.1, 2.3, 135.0},
        {5.6, 2.4, 136.0},
        {5.5, 1.8, 137.0},
        {4.8, 1.8, 138.0},
        {5.4, 2.1, 139.0},
        {5.6, 2.4, 140.0},
        {5.1, 2.3, 141.0},
        {5.1, 1.9, 142.0},
        {5.9, 2.3, 143.0},
        {5.7, 2.5, 144.0},
        {5.2, 2.3, 145.0},
        {5.0, 1.9, 146.0},
        {5.2, 2.0, 147.0},
        {5.4, 2.3, 148.0},
        {5.1, 1.8, 149.0}};
    float results[4];
    unsigned num_samples = 150;
#if PARALLEL
    // Configuração do número de threads
    omp_set_num_threads(threads);
#endif

    double total_execution_time = 0.0;
    double total_avg_prediction_time = 0.0;
    double min_execution_time = __DBL_MAX__;
    double max_execution_time = 0.0;

    for (int j = 0; j < num_reps; j++)
    {
        double start_time = omp_get_wtime();
#if PARALLEL
// Paralelização do loop principal com agendamento estático
#pragma omp parallel for
#endif
        for (int i = 0; i < num_samples; i++)
        {

            results[0] = svm_compute(samples[i], 2, set_vers_svs, set_vers_alphas, set_vers_bias);
            results[1] = svm_compute(samples[i], 2, set_virg_svs, set_virg_alphas, set_virg_bias);
            results[2] = svm_compute(samples[i], 16, versi_virg_svs, versi_virg_alphas, versi_virg_bias);
            results[3] = samples[i][2]; //id
// #if PARALLEL
// #pragma omp critical
// #endif
//             {
//                 printf("%3d: ", i);
//                 printf("%5f, ", results[0]);
//                 printf("%5f, ", results[1]);
//                 printf("%5f\n", results[2]);
//                 printf("Final class -> %d\n", classify(results));
//             }
        }
        double rep_total_time = omp_get_wtime() - start_time;
        double rep_avg_time = rep_total_time / num_samples;

        // Accumulate times for average calculation
        total_execution_time += rep_total_time;
        total_avg_prediction_time += rep_avg_time;

        // Update min and max times
        if (rep_total_time < min_execution_time)
            min_execution_time = rep_total_time;
        if (rep_total_time > max_execution_time)
            max_execution_time = rep_total_time;
    }
    // Calculate averages
    double avg_execution_time = total_execution_time / num_reps;
    double avg_prediction_time = total_avg_prediction_time / num_reps;

    // Print final results
    printf("\n=== RESULTADOS (Media de %d repeticoes) ===\n", num_reps);
#if PARALLEL
    printf("------ PARALELO ------\n");
#endif
    printf("threads disponiveis: %d, Utilizadas: %d\n", omp_get_max_threads(), threads);
    printf("Tempo medio por predicao: %.6f segundos\n", avg_prediction_time);
    printf("Tempo total medio de execucao: %.6f segundos\n", avg_execution_time);
    printf("Tempo total minimo: %.6f segundos\n", min_execution_time);
    printf("Tempo total maximo: %.6f segundos\n", max_execution_time);
}