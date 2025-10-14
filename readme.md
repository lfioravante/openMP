## Parallel OpenMP project

# Paralelização de Modelos de Machine Learning para Sistemas Embarcados

Este repositório contém a resolução do projeto final da disciplina de **Paralelização de Modelos de Machine Learning para Sistemas Embarcados** da UNIFEI. O objetivo principal do projeto é aplicar o OpenMP para paralelizar e otimizar o desempenho de algoritmos de multiplicação de matrizes e de modelos de Machine Learning.

---

## 📝 Visão Geral do Projeto

O projeto está estruturado em duas partes principais, conforme descrito nas aulas e no material de apoio:

1.  **Parte 1: Multiplicação de Matrizes:** Focada em analisar o ganho de desempenho (speedup) ao paralelizar um algoritmo de multiplicação de matrizes.
2.  **Parte 2: Algoritmos de Machine Learning:** Aplicação de diretivas OpenMP para paralelizar três algoritmos de ML (ANN, SVM e RF) e um modelo de CNN da biblioteca CMSIS-NN.

---
## 🛠️ Ambiente de Execução

Os códigos foram compilados e executados em diferentes ambientes:
* **Parte 1:** WSL - i72630QM
* **Parte 2:** Docker – QEMU ARM64 – Debian 12 - i72630QM

---

## 🚀 Parte 1: Multiplicação de Matrizes

### Objetivo

O principal objetivo desta etapa é se familiarizar com a programação paralela utilizando OpenMP e medir o ganho de desempenho da versão paralela em comparação com o código sequencial de referência. Os experimentos envolveram a variação do tamanho das matrizes e do número de threads para analisar o impacto no tempo de execução e no speedup.

### Implementações Desenvolvidas

Foram criadas duas implementações principais em C para a multiplicação de matrizes:

* **Matriz de Tamanho Fixo:** Uma implementação com matrizes de dimensão 4x4.
* **Matriz de Tamanho Variável (`matrix_generic.c`):** Uma implementação mais genérica que permite a multiplicação de matrizes de qualquer dimensão (`N x N`), alocadas dinamicamente. O número de threads também é configurável em tempo de execução.

Para garantir a corretude da multiplicação na versão de matriz variável, a Matriz B foi preenchida como uma matriz identidade, permitindo a verificação do resultado final com a Matriz A, já que `A x I = A`.

### Estratégia de Paralelização

A paralelização foi focada no loop de cálculo da multiplicação das matrizes.
* Foi utilizada a diretiva `#pragma omp parallel for collapse(2)` para paralelizar os dois loops externos aninhados (iterações de `i` e `j`), distribuindo o cálculo da matriz resultante entre as threads.
* O número de threads para a execução paralela é definido através de um argumento na linha de comando, utilizando a função `omp_set_num_threads(uThreads)`.
* Todos os códigos, incluindo a versão sequencial, foram compilados com a flag do OpenMP para permitir o uso da função `omp_get_wtime()` para uma medição precisa do tempo de execução.

### Compilação e Execução

1.  Navegue até a pasta `part1`:
    ```bash
    cd part1
    ```
2.  Compile todos os códigos com o comando:
    ```bash
    make all
    ```
3.  Execute os códigos conforme os exemplos abaixo:

    * **Matriz Fixa (Sequencial):**
        ```bash
        ./matrix_fixed_seq [Num_Repetições]
        ```
    * **Matriz Fixa (Paralelo):**
        ```bash
        ./matrix_fixed_par [Num_Repetições] [Num_Threads]
        ```
    * **Matriz Variável (Sequencial):**
        ```bash
        ./matrix_multiply_seq [Num_Repetições] [Tamanho_Matriz]
        ```
    * **Matriz Variável (Paralelo):**
        ```bash
        ./matrix_multiply_par [Num_Repetições] [Tamanho_Matriz] [Num_Threads]
        ```

### Análise de Resultados

#### Matriz Fixa (4x4)
Para matrizes de dimensão pequena, o overhead (custo de criação e gerenciamento das threads) da paralelização foi maior que o ganho de desempenho. Como resultado, a execução sequencial se mostrou mais eficiente.

* **Output Sequencial (1000 repetições):**
    * **Tempo médio:** 0.000149 ms
 
<img width="886" height="476" alt="image" src="https://github.com/user-attachments/assets/6a67cb80-3094-49a4-b24b-8687508f86a0" />

* **Output Paralelo (1000 repetições, 4 Threads):**
    * **Tempo médio:** 0.002549 ms

<img width="886" height="458" alt="image" src="https://github.com/user-attachments/assets/705395b9-f133-42fb-8197-d2091a97dcf3" />

#### Matriz Variável (Ex.:500x500)
Com o aumento da carga de trabalho (matrizes 500x500), os benefícios da paralelização tornaram-se evidentes, com uma melhora significativa no tempo de execução. A verificação do resultado (`A x I = A`) confirmou que a lógica paralela estava correta.

* **Output Sequencial (10 repetições):**
    * **Tempo médio:** 464.091446 ms
      
<img width="886" height="275" alt="image" src="https://github.com/user-attachments/assets/ee201b1f-eb82-4c01-8a59-4d640cb47ef5" />

* **Output Paralelo (10 repetições, 4 Threads):**
    * **Tempo médio:** 281.253110 ms

<img width="886" height="252" alt="image" src="https://github.com/user-attachments/assets/5ae26b42-ab7d-41a7-9fc7-01ae62775577" />

---

## 🧠 Parte 2: Paralelização de Algoritmos de Machine Learning

### Objetivo

O desafio desta parte é aplicar as técnicas de paralelização com OpenMP em algoritmos de Machine Learning mais complexos, visando otimizar suas fases de inferência.

### Implementações Desenvolvidas

Foram desenvolvidas as versões paralelas dos seguintes algoritmos:

1.  **Artificial Neural Network (ANN):** Paralelização do processo de classificação em uma rede neural, onde os neurônios são organizados em camadas.
2.  **Support Vector Machine (SVM):** Otimização de um classificador linear que separa os dados em duas classes distintas.
3.  **Random Forest (RF):** Paralelização do processo de votação, onde um vetor de entrada é classificado por múltiplas árvores de decisão binária.
4.  **Convolutional Neural Network (CNN) com CMSIS-NN:** Adaptação e paralelização de um modelo de CNN para o dataset **CIFAR-10**, utilizando a biblioteca CMSIS-NN como base. O trabalho focou em identificar e otimizar as regiões de código mais custosas computacionalmente.
