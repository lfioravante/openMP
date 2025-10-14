# Compiler and flags
CC = gcc
CFLAGS = -O3 -Wall -std=c99 -g
OMP_FLAG = -fopenmp
# CORRIGIDO: Adicionada a biblioteca matemática para funções como exp()
LDLIBS = -lm

# Source Directories
NN_DIR = NN
RF_DIR = RF
SVM_DIR = SVM

TARGET_NN_SEQ = nn_seq
TARGET_NN_PAR = nn_par
TARGET_RF_SEQ = rf_seq
TARGET_RF_PAR = rf_par
TARGET_SVM_SEQ = svm_seq
TARGET_SVM_PAR = svm_par

# Default parameters
PARALLEL ?= 0

# --- Neural Network Build Rules ---

$(TARGET_NN_SEQ): $(NN_DIR)/main.c
	$(CC) $(CFLAGS) -DPARALLEL=0 -o $@ $< $(LDLIBS) $(OMP_FLAG)

$(TARGET_NN_PAR): $(NN_DIR)/main.c
	$(CC) $(CFLAGS) -DPARALLEL=1 -o $@ $< $(LDLIBS) $(OMP_FLAG)

# --- Random Forest Build Rules ---

$(TARGET_RF_SEQ): $(RF_DIR)/main.c
	$(CC) $(CFLAGS) $(OMP_FLAG) -DPARALLEL=0 -o $@ $< $(LDLIBS)

$(TARGET_RF_PAR): $(RF_DIR)/main.c
	$(CC) $(CFLAGS) $(OMP_FLAG) -DPARALLEL=1 -o $@ $< $(LDLIBS)

# --- SVM Build Rules ---

$(TARGET_SVN_SEQ): $(SVM_DIR)/main.c
	$(CC) $(CFLAGS) $(OMP_FLAG) -DPARALLEL=0 -o $@ $< $(LDLIBS)

$(TARGET_SVM_PAR): $(SVM_DIR)/main.c
	$(CC) $(CFLAGS) $(OMP_FLAG) -DPARALLEL=1 -o $@ $< $(LDLIBS)


all_seq: $(TARGET_NN_SEQ) $(TARGET_RF_SEQ) $(TARGET_SVN_SEQ)

all_par: $(TARGET_NN_PAR) $(TARGET_RF_PAR) $(TARGET_SVM_PAR)

all: all_seq all_par


# --- Run Rules ---

run_nn:
ifeq ($(PARALLEL),1)
	@$(MAKE) $(TARGET_NN_PAR)
else
	@$(MAKE) $(TARGET_NN_SEQ)
endif

run_rf:
ifeq ($(PARALLEL),1)
	@$(MAKE) $(TARGET_RF_PAR)
else
	@$(MAKE) $(TARGET_RF_SEQ)
endif

run_svm:
ifeq ($(PARALLEL),1)
	@$(MAKE) $(TARGET_SVM_PAR)
else
	@$(MAKE) $(TARGET_SVN_SEQ)
endif


# --- Maintenance ---

clean:
	rm -f $(TARGET_NN_SEQ) $(TARGET_NN_PAR) $(TARGET_RF_SEQ) $(TARGET_RF_PAR) $(TARGET_SVN_SEQ) $(TARGET_SVM_PAR) *.o

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