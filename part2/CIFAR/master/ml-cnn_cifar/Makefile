CMSIS_DIR    = ../CMSIS_5
DSP_DIR		 = $(CMSIS_DIR)/CMSIS/DSP
NN_DIR    	 = $(CMSIS_DIR)/CMSIS/NN
CORE_DIR     = $(CMSIS_DIR)/CMSIS/Core/Include

APPINCLUDE:=-I$(APP_ROOT) \
			-I$(APP_ROOT)/Include \
			-I$(CORE_DIR) \
			-I$(DSP_DIR)/Include \
			-I$(NN_DIR)/Include \

VERBOSE=1
WORKING_FOLDER=$(shell pwd)
TOOL_CHAIN=gcc

APP_ROOT?=$(WORKING_FOLDER)

MULT?=
DEF?=

NUM_REPS ?= 100

-include NN.makefile.include

GNG_CC=g++
SRCINC?=
ASSDBG=
INCOBJ=$(patsubst %.c,%$(MULT).$(CROSS).o,$(SRCINC))
EXE=$(patsubst %.c,%.o,$(SRC))
EXEPP=$(patsubst %.cpp,%.o,$(SRCPP))
CFLAGS += -O3 
CFLAGS += -w -g 
CFLAGS += -fopenmp
VAR = -DPARALLEL=$(PARALLEL)
#CFLAGS += -std=gnu99

SRC+= $(wildcard *.c)

SRCPP+= $(wildcard *.cpp)

SRCOBJ:= $(patsubst %.c,%.o,$(SRC))

#
# ---------------------------------------------------------------------------------------
all: all_$(TOOL_CHAIN)

# ---------------------------------------------------------------------------------------
# gcc
ifeq ($(TOOL_CHAIN),gcc)

run_CFR: exe

exe::
	
ifeq ($(PARALLEL),1)
	# Building parallel version
	$(V) $(GNG_CC) $(CFLAGS) \
		$(SRC) \
		$(APPINCLUDE) $(VAR) -o par_app.exe
else
	# Building sequential version  
	$(V) $(GNG_CC) $(CFLAGS) \
		$(SRC) \
		$(APPINCLUDE) $(VAR) -o seq_app.exe

endif

# ---------------------------------------------------------------------------------------

clean::
	- rm -f *.$(CROSS).elf *.$(CROSS).o $(NN_DIR)/Source/*/*.o $(COREA_DIR)/Source/*.o $(DEVICE_DIR)/Source/*.o app_seq.exe app_par.exe
