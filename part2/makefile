# Nome do compilador C
CC = gcc

# --- Configurações de Compilação ---
CFLAGS = -Wall -Wextra -std=c99 -fopenmp

# --- Flags de Linkagem ---
# Adicionamos -lm para incluir a biblioteca matemática (para a função exp(), etc.)
LDFLAGS = -lm -fopenmp

# Perfil de Otimização (padrão)
ifeq ($(DEBUG), 1)
	CFLAGS += -g -O0
	BUILD_PROFILE = Depuração
else
	CFLAGS += -O3 -march=native
	BUILD_PROFILE = Otimizado (Release)
endif

# Compilação Paralela com OpenMP
ifeq ($(PARALLEL), 1)
	CFLAGS += -DPARALLEL=1
	PARALLEL_MODE = Paralelo (com OpenMP)
	
	# Configuração do número de threads
	THREADS ?= 4  # Valor padrão: 4 threads
	CFLAGS += -DTHREADS=$(THREADS)
	THREADS_INFO = , Threads: $(THREADS)
else
	CFLAGS += -DPARALLEL=0
	PARALLEL_MODE = Sequencial (apenas timing com OpenMP)
	THREADS_INFO = 
endif

# --- Estrutura do Projeto ---
PROGRAMS = NN RF SVM
TARGETS = $(foreach prog,$(PROGRAMS),$(prog)/$(prog))

# --- Regras Principais ---

# Regra 'all': compila todos os programas
.PHONY: all
all: $(TARGETS)
	@echo "\n[INFO] Todos os programas foram compilados com sucesso."

# Regra 'run': compila e executa todos os programas
.PHONY: run
run: $(TARGETS)
	@echo "\n[INFO] Executando todos os programas..."
	@for program in $(TARGETS); do \
		echo "\n--- Executando $$program ---"; \
		./$$program; \
		echo "--- Fim da execução de $$program ---"; \
	done

# --- Regras de Atalho e Limpeza ---
.PHONY: NN RF SVM clean run-NN run-RF run-SVM build-NN build-RF build-SVM

# Regras de compilação individual
build-NN: NN/NN
build-RF: RF/RF
build-SVM: SVM/SVM

# Regras de execução individual
run-NN: NN/NN
	@echo "--- Executando NN (Modo: $(PARALLEL_MODE)$(THREADS_INFO)) ---"
	OMP_NUM_THREADS=$(THREADS) ./NN/NN

run-RF: RF/RF
	@echo "--- Executando RF (Modo: $(PARALLEL_MODE)$(THREADS_INFO)) ---"
	OMP_NUM_THREADS=$(THREADS) ./RF/RF

run-SVM: SVM/SVM
	@echo "--- Executando SVM (Modo: $(PARALLEL_MODE)$(THREADS_INFO)) ---"
	OMP_NUM_THREADS=$(THREADS) ./SVM/SVM

# Regras para compilar e executar um programa específico
NN: build-NN run-NN
RF: build-RF run-RF
SVM: build-SVM run-SVM

clean:
	@echo "Limpando arquivos compilados..."
	rm -f $(TARGETS)

# --- Regras de Compilação Explícitas ---

NN/NN: NN/main.c
	@echo "--- Compilando NN | Perfil: $(BUILD_PROFILE) | Modo: $(PARALLEL_MODE)$(THREADS_INFO) ---"
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

RF/RF: RF/main.c
	@echo "--- Compilando RF | Perfil: $(BUILD_PROFILE) | Modo: $(PARALLEL_MODE)$(THREADS_INFO) ---"
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

SVM/SVM: SVM/main.c
	@echo "--- Compilando SVM | Perfil: $(BUILD_PROFILE) | Modo: $(PARALLEL_MODE)$(THREADS_INFO) ---"
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)