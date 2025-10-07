/* ----------------------------------------------------------------------
* Copyright (C) 2010-2018 Arm Limited. All rights reserved.
*
*
* Project:       CMSIS NN Library
* Title:         arm_nnexamples_cifar10.cpp
*
* Description:   Convolutional Neural Network Example
*
* Target Processor: Cortex-M4/Cortex-M7
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
* - Redistributions of source code must retain the above copyright
* notice, this list of conditions and the following disclaimer.
* - Redistributions in binary form must reproduce the above copyright
* notice, this list of conditions and the following disclaimer in
* the documentation and/or other materials provided with the
* distribution.
* - Neither the name of Arm LIMITED nor the names of its contributors
* may be used to endorse or promote products derived from this
* software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
* ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
* -------------------------------------------------------------------- */

#include <stdint.h>
#include <stdio.h>
#include "arm_math.h"
#include "arm_nnexamples_cifar10_parameter.h"
#include "arm_nnexamples_cifar10_weights.h"

#include "arm_nnfunctions.h"
#include "arm_nnexamples_cifar10_inputs.h"

// Inclui o cabeçalho do OpenMP para paralelização e medição de tempo
#include <omp.h>

// Define o número de vezes que a inferência será executada para calcular a média
#define NUM_REPETITIONS 10

// include the input and weights

static q7_t conv1_wt[CONV1_IM_CH * CONV1_KER_DIM * CONV1_KER_DIM * CONV1_OUT_CH] = CONV1_WT;
static q7_t conv1_bias[CONV1_OUT_CH] = CONV1_BIAS;

static q7_t conv2_wt[CONV2_IM_CH * CONV2_KER_DIM * CONV2_KER_DIM * CONV2_OUT_CH] = CONV2_WT;
static q7_t conv2_bias[CONV2_OUT_CH] = CONV2_BIAS;

static q7_t conv3_wt[CONV3_IM_CH * CONV3_KER_DIM * CONV3_KER_DIM * CONV3_OUT_CH] = CONV3_WT;
static q7_t conv3_bias[CONV3_OUT_CH] = CONV3_BIAS;

static q7_t ip1_wt[IP1_DIM * IP1_OUT] = IP1_WT;
static q7_t ip1_bias[IP1_OUT] = IP1_BIAS;

/* Here the image_data should be the raw uint8 type RGB image in [RGB, RGB, RGB ... RGB] format */
uint8_t   image_data[CONV1_IM_CH * CONV1_IM_DIM * CONV1_IM_DIM] = IMG_DATA;
q7_t      output_data[IP1_OUT];

//vector buffer: max(im2col buffer,average pool buffer, fully connected buffer)
q7_t      col_buffer[2 * 5 * 5 * 32 * 2];

q7_t      scratch_buffer[32 * 32 * 10 * 4];

int main()
{
    printf("Startup\n");

    // Variáveis para medição de tempo
    double start_time, end_time;
    double total_time_conv1 = 0, total_time_pool1 = 0;
    double total_time_conv2 = 0, total_time_pool2 = 0;
    double total_time_conv3 = 0, total_time_pool3 = 0;
    double total_time_fc = 0, total_time_softmax = 0;
    double total_inference_time = 0;

    printf("Executing inference %d times to get average...\n\n", NUM_REPETITIONS);

    for (int rep = 0; rep < NUM_REPETITIONS; rep++)
    {
        double repetition_start_time = omp_get_wtime();

        q7_t *img_buffer1 = scratch_buffer;
        q7_t *img_buffer2 = img_buffer1 + 32 * 32 * 32;

        /* input pre-processing */
        int mean_data[3] = INPUT_MEAN_SHIFT;
        unsigned int scale_data[3] = INPUT_RIGHT_SHIFT;
        for (int i = 0; i < 32 * 32 * 3; i += 3)
        {
            img_buffer2[i] = (q7_t)__SSAT(((((int)image_data[i] - mean_data[0]) << 7) + (0x1 << (scale_data[0] - 1))) >> scale_data[0], 8);
            img_buffer2[i + 1] = (q7_t)__SSAT(((((int)image_data[i + 1] - mean_data[1]) << 7) + (0x1 << (scale_data[1] - 1))) >> scale_data[1], 8);
            img_buffer2[i + 2] = (q7_t)__SSAT(((((int)image_data[i + 2] - mean_data[2]) << 7) + (0x1 << (scale_data[2] - 1))) >> scale_data[2], 8);
        }

        // Layer 1: Conv1 + ReLU
        start_time = omp_get_wtime();
        arm_convolve_HWC_q7_RGB(img_buffer2, CONV1_IM_DIM, CONV1_IM_CH, conv1_wt, CONV1_OUT_CH, CONV1_KER_DIM, CONV1_PADDING, CONV1_STRIDE, conv1_bias, CONV1_BIAS_LSHIFT, CONV1_OUT_RSHIFT, img_buffer1, CONV1_OUT_DIM, (q15_t *)col_buffer, NULL);
        arm_relu_q7(img_buffer1, CONV1_OUT_DIM * CONV1_OUT_DIM * CONV1_OUT_CH);
        end_time = omp_get_wtime();
        total_time_conv1 += (end_time - start_time);

        // Layer 2: Pool1
        start_time = omp_get_wtime();
        arm_maxpool_q7_HWC(img_buffer1, CONV1_OUT_DIM, CONV1_OUT_CH, POOL1_KER_DIM, POOL1_PADDING, POOL1_STRIDE, POOL1_OUT_DIM, NULL, img_buffer2);
        end_time = omp_get_wtime();
        total_time_pool1 += (end_time - start_time);

        // Layer 3: Conv2 + ReLU
        start_time = omp_get_wtime();
        arm_convolve_HWC_q7_fast(img_buffer2, CONV2_IM_DIM, CONV2_IM_CH, conv2_wt, CONV2_OUT_CH, CONV2_KER_DIM, CONV2_PADDING, CONV2_STRIDE, conv2_bias, CONV2_BIAS_LSHIFT, CONV2_OUT_RSHIFT, img_buffer1, CONV2_OUT_DIM, (q15_t *)col_buffer, NULL);
        arm_relu_q7(img_buffer1, CONV2_OUT_DIM * CONV2_OUT_DIM * CONV2_OUT_CH);
        end_time = omp_get_wtime();
        total_time_conv2 += (end_time - start_time);

        // Layer 4: Pool2
        start_time = omp_get_wtime();
        arm_maxpool_q7_HWC(img_buffer1, CONV2_OUT_DIM, CONV2_OUT_CH, POOL2_KER_DIM, POOL2_PADDING, POOL2_STRIDE, POOL2_OUT_DIM, col_buffer, img_buffer2);
        end_time = omp_get_wtime();
        total_time_pool2 += (end_time - start_time);

        // Layer 5: Conv3 + ReLU
        start_time = omp_get_wtime();
        arm_convolve_HWC_q7_fast(img_buffer2, CONV3_IM_DIM, CONV3_IM_CH, conv3_wt, CONV3_OUT_CH, CONV3_KER_DIM, CONV3_PADDING, CONV3_STRIDE, conv3_bias, CONV3_BIAS_LSHIFT, CONV3_OUT_RSHIFT, img_buffer1, CONV3_OUT_DIM, (q15_t *)col_buffer, NULL);
        arm_relu_q7(img_buffer1, CONV3_OUT_DIM * CONV3_OUT_DIM * CONV3_OUT_CH);
        end_time = omp_get_wtime();
        total_time_conv3 += (end_time - start_time);

        // Layer 6: Pool3
        start_time = omp_get_wtime();
        arm_maxpool_q7_HWC(img_buffer1, CONV3_OUT_DIM, CONV3_OUT_CH, POOL3_KER_DIM, POOL3_PADDING, POOL3_STRIDE, POOL3_OUT_DIM, col_buffer, img_buffer2);
        end_time = omp_get_wtime();
        total_time_pool3 += (end_time - start_time);

        // Layer 7: Fully-Connected
        start_time = omp_get_wtime();
        arm_fully_connected_q7_opt(img_buffer2, ip1_wt, IP1_DIM, IP1_OUT, IP1_BIAS_LSHIFT, IP1_OUT_RSHIFT, ip1_bias, output_data, (q15_t *)img_buffer1);
        end_time = omp_get_wtime();
        total_time_fc += (end_time - start_time);
        
        // Layer 8: Softmax
        start_time = omp_get_wtime();
        arm_softmax_q7(output_data, 10, output_data);
        end_time = omp_get_wtime();
        total_time_softmax += (end_time - start_time);

        total_inference_time += (omp_get_wtime() - repetition_start_time);
    }

    // Calcular e imprimir tempos médios
    printf("---------------------------------------------------\n");
    printf("Average Execution Time Report (after %d runs)\n", NUM_REPETITIONS);
    printf("---------------------------------------------------\n");
    printf("Layer 1 (Conv1 + ReLU): %.6f ms\n", (total_time_conv1 / NUM_REPETITIONS) * 1000);
    printf("Layer 2 (MaxPool1)    : %.6f ms\n", (total_time_pool1 / NUM_REPETITIONS) * 1000);
    printf("Layer 3 (Conv2 + ReLU): %.6f ms\n", (total_time_conv2 / NUM_REPETITIONS) * 1000);
    printf("Layer 4 (MaxPool2)    : %.6f ms\n", (total_time_pool2 / NUM_REPETITIONS) * 1000);
    printf("Layer 5 (Conv3 + ReLU): %.6f ms\n", (total_time_conv3 / NUM_REPETITIONS) * 1000);
    printf("Layer 6 (MaxPool3)    : %.6f ms\n", (total_time_pool3 / NUM_REPETITIONS) * 1000);
    printf("Layer 7 (FC)          : %.6f ms\n", (total_time_fc / NUM_REPETITIONS) * 1000);
    printf("Layer 8 (Softmax)     : %.6f ms\n", (total_time_softmax / NUM_REPETITIONS) * 1000);
    printf("---------------------------------------------------\n");
    printf("Total Average Inference Time: %.6f ms\n", (total_inference_time / NUM_REPETITIONS) * 1000);
    printf("---------------------------------------------------\n\n");


    printf("Final Output classification (from last run):\n");
    for (int i = 0; i < 10; i++)
    {
        printf("%d: %d\n", i, output_data[i]);
    }
    printf("Application end!\n");

    return 0;
}