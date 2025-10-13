# Compiler and flags
# Usar 'gcc' é mais portável. Garanta que o compilador esteja no PATH do sistema.
CC = gcc
CFLAGS = -O2 -Wall -std=c99 -g
OMP_FLAG = -fopenmp

# Targets
TARGET_SEQ = matrix_multiply_seq
TARGET_PAR = matrix_multiply_par

TARGET_FSEQ = matrix_fixed_seq
TARGET_FPAR = matrix_fixed_par

# Default target
all: $(TARGET_SEQ) $(TARGET_PAR) $(TARGETF_PAR) $(TARGETF_SEQ)

# Todos estão compilados com o OpenMP para utilizar a função de tempo - omp_get_wtime()
# Cria um diretório 'tmp' e força o compilador a usá-lo para arquivos temporários
$(TARGET_SEQ): ./matrix_dimension/Generic_Serial_Matrix_Multi.c
	$(CC) $(CFLAGS) $(OMP_FLAG) -DPARALLEL=0  -o $@ $<

# Parallel version (with OpenMP)
$(TARGET_PAR): ./matrix_dimension/Generic_Serial_Matrix_Multi.c
	$(CC) $(CFLAGS) $(OMP_FLAG) -DPARALLEL=1 -o $@ $<

$(TARGETF_SEQ): ./matrix_fixed_input/Generic_Serial_Matrix_Fixed.c
	$(CC) $(CFLAGS) $(OMP_FLAG) -DPARALLEL=0  -o $@ $<

# Parallel version (with OpenMP)
$(TARGETF_PAR): ./matrix_fixed_input/Generic_Serial_Matrix_Fixed.c
	$(CC) $(CFLAGS) $(OMP_FLAG) -DPARALLEL=1 -o $@ $<

# Run sequential version
# CORRIGIDO: Removido '.exe' para compatibilidade com Linux/WSL
run_seq: $(TARGET_SEQ) $(TARGETF_SEQ)

# Run parallel version
# CORRIGIDO: Removido '.exe' para compatibilidade com Linux/WSL
run_par: $(TARGET_PAR) $(TARGETF_PAR)

# Clean build artifacts
# CORRIGIDO: Removido '.exe' dos alvos a serem limpos
clean:
	rm -f $(TARGET_SEQ) $(TARGET_PAR) $(TARGETF_PAR) $(TARGETF_SEQ) *.o
	rm -rf tmp
