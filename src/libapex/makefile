# Define projects layout
INC_DIR=./include

# Define c++ compiler and Compile/Link FLAGS
CC=i686-elf-g++
AR=i686-elf-ar
CFLAGS=-ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -Wno-unused-variable -I$(INC_DIR)
CFLAGS_R=-O3
CFLAGS_D=-O0 -g -D_DEBUG
LFLAGS=-ffreestanding -O2 -nostdlib -lgcc
LFLAGS_R=
LFLAGS_D=
ARFLAGS=rcs
ARFLAGS_R=
ARFLAGS_D=

# Define c++ source, header, and object files
CPP_SOURCES=$(wildcard *.cpp)
CPP_HEADERS=$(wildcard *.hpp)
CPP_INC_HEADERS=$(addprefix $(INC_DIR)/, $(basename $(CPP_HEADERS)))
CPP_OBJECTS=$(CPP_SOURCES:.cpp=.co)
CPP_OBJECTS_D=$(CPP_SOURCES:.cpp=.co_d)

# Define NASM flags
NASM=nasm
NASM_FLAGS=-f elf32
NASM_FLAGS_D=-g

# Define asm source and object files
ASM_SOURCES=$(wildcard *.asm)
ASM_OBJECTS=$(ASM_SOURCES:.asm=.ao)
ASM_OBJECTS_D=$(ASM_SOURCES:.asm=.ao_d)

# Target list
.PHONY: list
list:
	@echo 'target(deps)       : Description'
	@echo
	@echo 'libapex-r          : Builds the common system framework'
	@echo 'libapex-d          : Builds the common system framework with debug symbols'
	@echo 'clean              : Cleans up all intermediate files'
	@echo 'distclean(clean)   : Cleans up all distributable files'
	@echo

# Copies all .hpp files into $(INC_DIR)
$(INC_DIR)/% : %.hpp
	mkdir -p $(INC_DIR) && true
	cp $< $@

# Build any .co file from it's .cpp file
%.co : %.cpp $(CPP_INC_HEADERS)
	$(CC) -c $< -o $@ $(CFLAGS) $(CFLAGS_R)

# Build any .ao file from it's .asm file
%.ao : %.asm
	$(NASM) $(NASM_FLAGS) -o $@ $<

# Build any .co_d file from it's .cpp file, but add debug flags
%.co_d : %.cpp $(CPP_INC_HEADERS)
	$(CC) -c $< -o $@ $(CFLAGS) $(CFLAGS_D)

# Build any .ao_d file from it's .asm file, but add debug flags
%.ao_d : %.asm
	$(NASM) $(NASM_FLAGS) $(NASM_FLAGS_D) -o $@ $<

# Build the common APEX library
libapex-r: $(ASM_OBJECTS) $(CPP_OBJECTS) $(CPP_INC_HEADERS)
	rm libapex.a || true
	$(AR) $(ARFLAGS) $(ARFLAGS_R) libapex.a $^

libapex-d: $(ASM_OBJECTS_D) $(CPP_OBJECTS_D) $(CPP_INC_HEADERS)
	rm libapex.a || true
	$(AR) $(ARFLAGS) $(ARFLAGS_D) libapex.a $^

# Clean rule
.PHONY: clean
clean:
	rm -f *.ao *.ao_d *.co *.co_d

# Cleans all virtual device targets
.PHONY: distclean
distclean: clean
	rm -f *.elf *.a
	rm -rf $(INC_DIR)
