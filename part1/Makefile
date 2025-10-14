# Compiler and flags
CC = gcc
CFLAGS = -O2 -Wall -std=c99 -g
OMP_FLAG = -fopenmp

# Targets
TARGET_SEQ = matrix_multiply_seq
TARGET_PAR = matrix_multiply_par

TARGET_FSEQ = matrix_fixed_seq
TARGET_FPAR = matrix_fixed_par

# Default target
all: $(TARGET_SEQ) $(TARGET_PAR) $(TARGET_FPAR) $(TARGET_FSEQ)

# Todos estão compilados com o OpenMP para utilizar a função de tempo - omp_get_wtime()
# Cria um diretório 'tmp' e força o compilador a usá-lo para arquivos temporários
$(TARGET_SEQ): ./matrix_dimension/Generic_Serial_Matrix_Multi.c
	$(CC) $(CFLAGS) $(OMP_FLAG) -DPARALLEL=0  -o $@ $<

# Parallel version (with OpenMP)
$(TARGET_PAR): ./matrix_dimension/Generic_Serial_Matrix_Multi.c
	$(CC) $(CFLAGS) $(OMP_FLAG) -DPARALLEL=1 -o $@ $<

$(TARGET_FSEQ): ./matrix_fixed_input/Generic_Serial_Matrix_Fixed.c
	$(CC) $(CFLAGS) $(OMP_FLAG) -DPARALLEL=0  -o $@ $<

# Parallel version (with OpenMP)
$(TARGET_FPAR): ./matrix_fixed_input/Generic_Serial_Matrix_Fixed.c
	$(CC) $(CFLAGS) $(OMP_FLAG) -DPARALLEL=1 -o $@ $<

# Run sequential version
run_seq: $(TARGET_SEQ) $(TARGET_FSEQ)

# Run parallel version
run_par: $(TARGET_PAR) $(TARGET_FPAR)

# Clean build artifacts
clean:
	rm -f $(TARGET_SEQ) $(TARGET_PAR) $(TARGET_FPAR) $(TARGET_FSEQ) *.o
	rm -rf tmp
