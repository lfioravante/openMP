# Compiler and flags
CC = gcc
CFLAGS = -O2 -Wall -std=c99 -g
OMP_FLAG = -fopenmp
# CORRIGIDO: Adicionada a biblioteca matemática para funções como exp()
LDLIBS = -lm

# Source Directories
NN_DIR = NN
RF_DIR = RF
SVM_DIR = SVM


TARGET_NN = nn.exe
TARGET_RF = rf.exe
TARGET_SVM = svm.exe

# Default parameters
THREADS ?= 4
PARALLEL ?= 1
NUM_REPS ?= 10

# Default target
all: $(TARGET_NN) $(TARGET_RF)  $(TARGET_SVM)

# --- Build Rules ---

$(TARGET_NN): $(NN_DIR)/main.c
	$(CC) $(CFLAGS) $(OMP_FLAG) -DPARALLEL=$(PARALLEL) -DTHREADS=$(THREADS) -DNUM_REPS=$(NUM_REPS) -o $@ $< $(LDLIBS)

$(TARGET_RF): $(RF_DIR)/main.c
	$(CC) $(CFLAGS) $(OMP_FLAG) -DPARALLEL=$(PARALLEL) -DTHREADS=$(THREADS) -DNUM_REPS=$(NUM_REPS) -o $@ $< $(LDLIBS)

$(TARGET_SVM): $(SVM_DIR)/main.c
	$(CC) $(CFLAGS) $(OMP_FLAG) -DPARALLEL=$(PARALLEL) -DTHREADS=$(THREADS) -DNUM_REPS=$(NUM_REPS) -o $@ $< $(LDLIBS)


# --- Run Rules ---

run_nn: clean
	@$(MAKE) $(TARGET_NN) > /dev/null
	./$(TARGET_NN) $(THREADS)

run_rf: clean
	@$(MAKE) $(TARGET_RF) > /dev/null
	./$(TARGET_RF) $(THREADS)

run_svm: clean
	@$(MAKE) $(TARGET_SVM) > /dev/null
	./$(TARGET_SVM) $(THREADS)


# --- Maintenance ---

clean:
	rm -f $(TARGET_NN) $(TARGET_RF) $(TARGET_SVM) *.o

help:
	@echo "Available targets:"
	@echo "  all        - Build all sequential and parallel versions"
	@echo "  run_nn     - Run NN version. Default is PARALLEL."
	@echo "  run_rf     - Run RF version. Default is PARALLEL."
	@echo "  run_svm    - Run SVM version. Default is PARALLEL."
	@echo "  clean      - Remove compiled binaries"
	@echo ""
	@echo "How to run sequential version:"
	@echo "  make run_nn PARALLEL=0"
	@echo ""
	@echo "Override defaults for parallel run:"
	@echo "  make run_nn THREADS=8"

.PHONY: all run_nn run_rf run_svm clean help