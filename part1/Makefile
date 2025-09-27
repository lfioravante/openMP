# Compiler and flags
# Usar 'gcc' é mais portável. Garanta que o compilador esteja no PATH do sistema.
CC = gcc
CFLAGS = -O2 -Wall -std=c99 -g
OMP_FLAG = -fopenmp

# Targets
TARGET_SEQ = matrix_multiply_seq
TARGET_PAR = matrix_multiply_par

# Default parameters (can be overridden)
MATRIX_SIZE ?= 1000
THREADS ?= 4
REPETITIONS ?= 10

# Default target
all: $(TARGET_SEQ) $(TARGET_PAR)

# Sequential version (no OpenMP)
# Cria um diretório 'tmp' e força o compilador a usá-lo para arquivos temporários
$(TARGET_SEQ): Generic_Serial_Matrix_Multi.c
	$(CC) $(CFLAGS) -DPARALLEL=0 -DREPETITIONS=$(REPETITIONS) -o $@ $<

# Parallel version (with OpenMP)
$(TARGET_PAR): Generic_Serial_Matrix_Multi.c
	$(CC) $(CFLAGS) $(OMP_FLAG) -DPARALLEL=1 -DREPETITIONS=$(REPETITIONS) -o $@ $<

# Run sequential version
# CORRIGIDO: Removido '.exe' para compatibilidade com Linux/WSL
run_seq: $(TARGET_SEQ)
	./$(TARGET_SEQ) $(MATRIX_SIZE) $(THREADS)

# Run parallel version
# CORRIGIDO: Removido '.exe' para compatibilidade com Linux/WSL
run_par: $(TARGET_PAR)
	./$(TARGET_PAR) $(MATRIX_SIZE) $(THREADS)

# Clean build artifacts
# CORRIGIDO: Removido '.exe' dos alvos a serem limpos
clean:
	rm -f $(TARGET_SEQ) $(TARGET_PAR) *.o
	rm -rf tmp

# Help information
help:
	@echo "Available targets:"
	@echo "  all              - Build both sequential and parallel versions"
	@echo "  run_seq          - Run sequential version (iSize=$(MATRIX_SIZE))"
	@echo "  run_par          - Run parallel version (iSize=$(MATRIX_SIZE), iThreads=$(THREADS))"
	@echo "  clean            - Remove compiled binaries"
	@echo ""
	@echo "Override defaults:"
	@echo "  make run_par MATRIX_SIZE=2000 THREADS=8 REPETITIONS=15"

.PHONY: all run_seq run_par clean help