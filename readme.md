# Paralelização de Modelos de Machine Learning para Sistemas Embarcados

## Este repositório contém a resolução do projeto final do curso de **Paralelização de Modelos de Machine Learning para Sistemas Embarcados** do Professor Luciano Ost, aplicado na UNIFEI. O objetivo principal do projeto é aplicar o OpenMP para paralelizar e otimizar o desempenho de algoritmos de multiplicação de matrizes e de modelos de Machine Learning.

---

## 🌎 Visão Geral do Projeto

O projeto está estruturado em duas partes principais, conforme descrito nas aulas e no material de apoio:

1.  **Parte 1: Multiplicação de Matrizes:** Focada em analisar o ganho de desempenho (speedup) ao paralelizar um algoritmo de multiplicação de matrizes.
2.  **Parte 2: Algoritmos de Machine Learning:** Aplicação de diretivas OpenMP para paralelizar três algoritmos de ML (ANN, SVM e RF) e um modelo de CNN da biblioteca CMSIS-NN.
3.  **Parte 3: Vídeo de explicação do projeto e resultados**

---
## 💻 Ambiente de Execução

Os códigos foram compilados e executados em diferentes ambientes:
* **Parte 1:** WSL - i72630QM
* **Parte 2:** Container Docker – QEMU ARM64 – Debian 12 - i72630QM

---

## 🔢 Parte 1: Multiplicação de Matrizes

### Objetivo

O principal objetivo desta etapa é se familiarizar com a programação paralela utilizando OpenMP e medir o ganho de desempenho da versão paralela em comparação com o código sequencial de referência. Os experimentos envolveram a variação do tamanho das matrizes e do número de threads para analisar o impacto no tempo de execução e no speedup.

### Implementações Desenvolvidas

Foram criadas duas implementações principais em C para a multiplicação de matrizes:

* **Matriz de Tamanho Fixo:** Uma implementação com matrizes de dimensão 4x4.
* **Matriz de Tamanho Variável:** Uma implementação mais genérica que permite a multiplicação de matrizes de qualquer dimensão (`N x N`), alocadas dinamicamente. O número de threads também é configurável em tempo de execução.

Para garantir a corretude da multiplicação na versão de **matriz variável**, a Matriz B foi preenchida como uma matriz identidade, permitindo a verificação do resultado final com a Matriz A, já que `A x I = A`.

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
 
<img width="886" height="476" alt="image" src="https://github.com/user-attachments/assets/7b5d5af1-3d2f-4f81-8f70-e348037077c0" />

* **Output Paralelo (1000 repetições, 4 Threads):**
    * **Tempo médio:** 0.002549 ms

<img width="886" height="458" alt="image" src="https://github.com/user-attachments/assets/cc470cd0-012f-443f-85fa-d54e42b21bae" />

#### Matriz Variável (Ex.:500x500)
Com o aumento da carga de trabalho (matrizes 500x500), os benefícios da paralelização tornaram-se evidentes, com uma melhora significativa no tempo de execução. A verificação do resultado (`A x I = A`) confirmou que a lógica paralela estava correta.

* **Output Sequencial (10 repetições):**
    * **Tempo médio:** 464.091446 ms
      
<img width="886" height="275" alt="image" src="https://github.com/user-attachments/assets/690067cd-4124-4b52-9b7c-b4ec6a4fbe56" />

* **Output Paralelo (10 repetições, 4 Threads):**
    * **Tempo médio:** 281.253110 ms

<img width="886" height="252" alt="image" src="https://github.com/user-attachments/assets/6831feb0-821a-4e34-9b40-895f561fabcb" />

---

## 🤖 Parte 2: Paralelização de Algoritmos de Machine Learning

### Objetivo

O desafio desta parte foi aplicar as técnicas de paralelização com OpenMP em algoritmos de Machine Learning mais complexos, visando otimizar suas fases de inferência. Devido a baixa carga de processamento dos algoritmos e do modelo, os códigos foram executados em um container docker com o QEMU emulando uma raspberry pi, devido a conversão de 64-bits para ARM64, o desempenho da CPU cai drasticamente sendo possivel observar melhor os resultados e se aproximando de um sistema embarcado. Specs: 4 CPUs e 8Gb de RAM no RaspBerry Pi OS Bookworm(Debian 12).

Os prints foram ocultados para melhor observação dos resultados.

### Estratégia de Paralelização

As estratégias de paralelização foram adaptadas para as características de cada algoritmo:

1.  **Artificial Neural Network (ANN):**
    * O loop principal de inferência sobre o conjunto de teste foi paralelizado com `#pragma omp parallel for`.
    * Foi utilizado `schedule(dynamic)` para balancear a carga entre as threads, já que o tempo de processamento por amostra pode variar.
    * A cláusula `reduction(+: correct_predictions)` foi usada para somar de forma segura o número de predições corretas de cada thread.
    * A diretiva `simd` foi aplicada em loops internos para vetorizar cálculos, como a busca pelo score máximo na camada de saída.

2.  **Support Vector Machine (SVM):**
    * A paralelização foi focada no loop principal que itera sobre as amostras de teste, aplicando `#pragma omp parallel for`.

3.  **Random Forest (RF):**
    * Diversas partes do algoritmo foram otimizadas:
        * **Carregamento do Modelo:** O processo de leitura e alocação das árvores em memória foi paralelizado para acelerar a inicialização.
        * **Inferência:** O loop que avalia o conjunto de testes foi paralelizado. Cada thread processa um subconjunto de amostras de forma independente.
        * **Votação:** O cálculo do voto majoritário, que agrega as predições de todas as árvores, foi otimizado com `#pragma omp simd reduction` para realizar a contagem de votos de forma vetorizada e segura.

4.  **Convolutional Neural Network (CNN) com CMSIS-NN:**
    * A otimização concentrou-se nas funções mais custosas computacionalmente da biblioteca CMSIS-NN.
    * **Camadas de Convolução (`arm_convolve_...`):** Os loops que iteram sobre os canais de saída foram paralelizados com `#pragma omp parallel for schedule(dynamic)`. Esta é a parte mais intensiva do modelo e onde o ganho de desempenho é mais significativo.
    * **Funções de Ativação (`arm_relu_...`):** A diretiva `#pragma omp simd` foi utilizada para vetorizar a aplicação da função ReLU, processando múltiplos valores do vetor de dados simultaneamente.

### Compilação e Execução

1.  Navegue até a pasta `part2`:
    ```bash
    cd part2
    ```
2.  **Para ANN, RF e SVM:**
    * Compile todos os códigos com o comando `make all`.
    * Execute os algoritmos com os seguintes comandos:
        ```bash
        # ANN Sequencial e Paralelo
        ./nn_seq [Num_Repetições]
        ./nn_par [Num_Repetições] [Num_Threads]

        # RF Sequencial e Paralelo
        ./rf_seq [Num_Repetições]
        ./rf_par [Num_Repetições] [Num_Threads]

        # SVM Sequencial e Paralelo
        ./svm_seq [Num_Repetições]
        ./svm_par [Num_Repetições] [Num_Threads]
        ```

3.  **Para a CNN:**
    * Navegue até a pasta do projeto: `cd CIFAR/master/ml-cnn_cifar`.
    * Compile e execute as versões:
        ```bash
        # Compilar e rodar versão sequencial
        make run_CFR PARALLEL=0
        ./seq_app [Num_Repetições]

        # Compilar e rodar versão paralela
        make run_CFR PARALLEL=1
        ./par_app [Num_Repetições] [Num_Threads]
        ```

### Análise de Resultados

#### Artificial Neural Network (ANN)
A paralelização se mostrou muito eficaz, reduzindo o tempo total de processamento de **2.09 segundos** para **0.84 segundos** (com 4 threads), um **speedup de aproximadamente 2.4x**. Isso demonstra que a carga de trabalho do algoritmo é suficiente para superar o overhead da paralelização.

* **Output Sequencial vs. Paralelo (1000 repetições, 4 Threads):**

<img width="791" height="619" alt="image" src="https://github.com/user-attachments/assets/afafe2f5-63c7-48d8-8bb0-98b8c4267038" />


#### Random Forest (RF)
Neste caso, a versão paralela foi mais lenta que a sequencial. O tempo total de execução aumentou de **0.0019 segundos** para **0.0038 segundos**. Isso indica que o custo de criar e gerenciar as threads (`overhead`) foi maior que o benefício obtido, pois o tempo de predição por amostra é extremamente baixo. A paralelização seria vantajosa para um conjunto de dados de teste muito maior ou para árvores mais complexas.

* **Output Sequencial vs. Paralelo (1000 repetições, 4 Threads):**

<img width="664" height="380" alt="image" src="https://github.com/user-attachments/assets/602951c1-15cc-4755-be3b-02d662469bbb" />
<img width="673" height="408" alt="image" src="https://github.com/user-attachments/assets/3ab49fc5-2506-491e-b11c-268fbb845625" />



#### Support Vector Machine (SVM)
Assim como no RF, o SVM apresentou um resultado onde a versão paralela foi menos eficiente. O tempo de execução, que já era muito baixo na versão sequencial (próximo de 0.000001s), aumentou na versão paralela. O motivo é o mesmo: a carga de trabalho por inferência é insuficiente para justificar o custo da paralelização.

* **Output Sequencial vs. Paralelo (1000 repetições, 4 Threads):**

<img width="819" height="483" alt="image" src="https://github.com/user-attachments/assets/f4a89797-a99e-4b86-9ae3-a91bdec50a4d" />

#### Convolutional Neural Network (CNN)
A paralelização da CNN resultou em um ganho de desempenho expressivo. O tempo total de inferência foi reduzido de **65.7 ms** para **32.6 ms** (com 4 threads), um **speedup de aproximadamente 2x**. A análise detalhada por camada mostra que o ganho se concentra nas camadas convolucionais (`Conv1 + ReLU`, `Conv2 + ReLU`), que são computacionalmente intensivas e foram o alvo principal das diretivas OpenMP.

* **Output Sequencial (100 repetições):**
<img width="886" height="402" alt="image" src="https://github.com/user-attachments/assets/cb0b7327-16ee-496d-b1fb-185ef9b7d382" />

* **Output Paralelo (100 repetições, 4 Threads):**
<img width="886" height="403" alt="image" src="https://github.com/user-attachments/assets/dad9da13-24d9-427b-bb08-d8f17ea39890" />
