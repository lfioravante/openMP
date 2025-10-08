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

int main(int argc, char *argv[]) {
    // Valores padrão
    int threads = 4;
    int num_reps = 100;
    
    // Ler argumentos da linha de comando
    if (argc >= 2) {
        threads = atoi(argv[1]);
    }
    if (argc >= 3) {
        num_reps = atoi(argv[2]);
    }

    float samples[150][2] = {
        {1.4, 0.2},
        {1.4, 0.2},
        {1.3, 0.2},
        {1.5, 0.2},
        {1.4, 0.2},
        {1.7, 0.4},
        {1.4, 0.3},
        {1.5, 0.2},
        {1.4, 0.2},
        {1.5, 0.1},
        {1.5, 0.2},
        {1.6, 0.2},
        {1.4, 0.1},
        {1.1, 0.1},
        {1.2, 0.2},
        {1.5, 0.4},
        {1.3, 0.4},
        {1.4, 0.3},
        {1.7, 0.3},
        {1.5, 0.3},
        {1.7, 0.2},
        {1.5, 0.4},
        {1.0, 0.2},
        {1.7, 0.5},
        {1.9, 0.2},
        {1.6, 0.2},
        {1.6, 0.4},
        {1.5, 0.2},
        {1.4, 0.2},
        {1.6, 0.2},
        {1.6, 0.2},
        {1.5, 0.4},
        {1.5, 0.1},
        {1.4, 0.2},
        {1.5, 0.1},
        {1.2, 0.2},
        {1.3, 0.2},
        {1.5, 0.1},
        {1.3, 0.2},
        {1.5, 0.2},
        {1.3, 0.3},
        {1.3, 0.3},
        {1.3, 0.2},
        {1.6, 0.6},
        {1.9, 0.4},
        {1.4, 0.3},
        {1.6, 0.2},
        {1.4, 0.2},
        {1.5, 0.2},
        {1.4, 0.2},
        {4.7, 1.4},
        {4.5, 1.5},
        {4.9, 1.5},
        {4.0, 1.3},
        {4.6, 1.5},
        {4.5, 1.3},
        {4.7, 1.6},
        {3.3, 1.0},
        {4.6, 1.3},
        {3.9, 1.4},
        {3.5, 1.0},
        {4.2, 1.5},
        {4.0, 1.0},
        {4.7, 1.4},
        {3.6, 1.3},
        {4.4, 1.4},
        {4.5, 1.5},
        {4.1, 1.0},
        {4.5, 1.5},
        {3.9, 1.1},
        {4.8, 1.8},
        {4.0, 1.3},
        {4.9, 1.5},
        {4.7, 1.2},
        {4.3, 1.3},
        {4.4, 1.4},
        {4.8, 1.4},
        {5.0, 1.7},
        {4.5, 1.5},
        {3.5, 1.0},
        {3.8, 1.1},
        {3.7, 1.0},
        {3.9, 1.2},
        {5.1, 1.6},
        {4.5, 1.5},
        {4.5, 1.6},
        {4.7, 1.5},
        {4.4, 1.3},
        {4.1, 1.3},
        {4.0, 1.3},
        {4.4, 1.2},
        {4.6, 1.4},
        {4.0, 1.2},
        {3.3, 1.0},
        {4.2, 1.3},
        {4.2, 1.2},
        {4.2, 1.3},
        {4.3, 1.3},
        {3.0, 1.1},
        {4.1, 1.3},
        {6.0, 2.5},
        {5.1, 1.9},
        {5.9, 2.1},
        {5.6, 1.8},
        {5.8, 2.2},
        {6.6, 2.1},
        {4.5, 1.7},
        {6.3, 1.8},
        {5.8, 1.8},
        {6.1, 2.5},
        {5.1, 2.0},
        {5.3, 1.9},
        {5.5, 2.1},
        {5.0, 2.0},
        {5.1, 2.4},
        {5.3, 2.3},
        {5.5, 1.8},
        {6.7, 2.2},
        {6.9, 2.3},
        {5.0, 1.5},
        {5.7, 2.3},
        {4.9, 2.0},
        {6.7, 2.0},
        {4.9, 1.8},
        {5.7, 2.1},
        {6.0, 1.8},
        {4.8, 1.8},
        {4.9, 1.8},
        {5.6, 2.1},
        {5.8, 1.6},
        {6.1, 1.9},
        {6.4, 2.0},
        {5.6, 2.2},
        {5.1, 1.5},
        {5.6, 1.4},
        {6.1, 2.3},
        {5.6, 2.4},
        {5.5, 1.8},
        {4.8, 1.8},
        {5.4, 2.1},
        {5.6, 2.4},
        {5.1, 2.3},
        {5.1, 1.9},
        {5.9, 2.3},
        {5.7, 2.5},
        {5.2, 2.3},
        {5.0, 1.9},
        {5.2, 2.0},
        {5.4, 2.3},
        {5.1, 1.8}};

    float results[3];
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
#if PARALLEL
#pragma omp critical
#endif
            {
                printf("%3d: ", i);
                printf("%5f, ", results[0]);
                printf("%5f, ", results[1]);
                printf("%5f\n", results[2]);
                printf("Final class -> %d\n", classify(results));
            }
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