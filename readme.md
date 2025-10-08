## Parallel OpenMP project

# Paralelização de Modelos de Machine Learning para Sistemas Embarcados

Este repositório contém a resolução do projeto final da disciplina de **Paralelização de Modelos de Machine Learning para Sistemas Embarcados** da UNIFEI. O objetivo principal do projeto é aplicar o OpenMP para paralelizar e otimizar o desempenho de algoritmos de multiplicação de matrizes e de modelos de Machine Learning.

---

## 📝 Visão Geral do Projeto

O projeto está estruturado em duas partes principais, conforme descrito nas aulas e no material de apoio:

1.  **Parte 1: Multiplicação de Matrizes:** Focada em analisar o ganho de desempenho (speedup) ao paralelizar um algoritmo de multiplicação de matrizes.
2.  **Parte 2: Algoritmos de Machine Learning:** Aplicação de diretivas OpenMP para paralelizar três algoritmos de ML (ANN, SVM e RF) e um modelo de CNN da biblioteca CMSIS-NN.

---

## 🚀 Parte 1: Multiplicação de Matrizes

### Objetivo

O principal objetivo desta etapa é se familiarizar com a programação paralela utilizando OpenMP e medir o ganho de desempenho da versão paralela em comparação com o código sequencial de referência. ]Os experimentos envolveram a variação do tamanho das matrizes e do número de threads para analisar o impacto no tempo de execução e no speedup.

### Implementações Desenvolvidas

Foram criadas duas implementações principais em C:

* **`matrix_generic.c`**: Uma implementação genérica que permite a multiplicação de matrizes de qualquer dimensão, com o número de threads sendo configurável em tempo de execução.

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
