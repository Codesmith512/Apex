# Define projects layout
INC_DIR=./include
PROJ_DEPS=libapex libapex++

# Define c++ compiler and Compile/Link FLAGS
CC=i686-elf-g++
IGNORE_WARNINGS=
CFLAGS=-std=c++17 -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti $(foreach warn,$(IGNORE_WARNINGS),-Wno-$(warn)) -I$(INC_DIR) $(foreach proj,$(PROJ_DEPS),-I./$(proj)/include)
CFLAGS_R=-O3
CFLAGS_D=-O0 -g -D_DEBUG
LFLAGS=-ffreestanding -O2 -nostdlib -lgcc
LFLAGS_D=
LIB_DEPS=$(foreach proj,$(PROJ_DEPS),./$(proj)/$(proj).a)

# Define c++ source, header, and object files
CPP_SOURCES=$(wildcard *.cpp)
CPP_HEADERS=$(wildcard *.hpp)
CPP_INC_HEADERS=$(addprefix $(INC_DIR)/, $(basename $(CPP_HEADERS)))
CPP_OBJECTS=$(CPP_SOURCES:.cpp=.co)
CPP_OBJECTS_D=$(CPP_SOURCES:.cpp=.co_d)

# C++ CRT* objects
CRTI_OBJ=crti.crto
CRTBEGIN_OBJ=$(shell $(CC) $(CFLAGS)  -print-file-name=crtbegin.o)
CRTEND_OBJ=$(shell $(CC) $(CFLAGS)  -print-file-name=crtend.o)
CRTN_OBJ=crtn.crto

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
	@echo 'libapex++-r        : Builds the C++ STL'
	@echo 'libapex++-d        : Builds the C++ STL with debug symbols'
	@echo 'kernel-r(libapex)  : Builds the kernel'
	@echo 'kernel-d(libapex)  : Builds the kernel with debug symbols'
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

# Build any .crto file from it's .crt file
%.crto : %.crt
	$(NASM) $(NASM_FLAGS) -o $@ $<

# Build any .co_d file from it's .cpp file, but add debug flags
%.co_d : %.cpp $(CPP_INC_HEADERS)
	$(CC) -c $< -o $@ $(CFLAGS) $(CFLAGS_D)

# Build any .ao_d file from it's .asm file, but add debug flags
%.ao_d : %.asm
	$(NASM) $(NASM_FLAGS) $(NASM_FLAGS_D) -o $@ $<

#Build the common system framework
.PHONY: libapex-r
libapex-r:
	make -C libapex libapex-r

.PHONY: libapex-d
libapex-d:
	make -C libapex libapex-d

# Build the C++ STL
.PHONY: libapex++-r
libapex++-r:
	make -C libapex++ libapex++-r

.PHONY: libapex++-d
libapex++-d:
	make -C libapex++ libapex++-d

# Build the kernel
kernel-r: libapex-r libapex++-r $(CRTI_OBJ) $(ASM_OBJECTS) $(CPP_OBJECTS) $(CRTN_OBJ) $(CPP_INC_HEADERS)
	rm kernel.elf || true
	$(CC) -T kernel.ld -o kernel.elf $(CRTI_OBJ) $(CRTBEGIN_OBJ) $(ASM_OBJECTS) $(CPP_OBJECTS) $(CRTEND_OBJ) $(CRTN_OBJ) $(LIB_DEPS) $(LFLAGS)

kernel-d: libapex-d libapex++-d $(CRTI_OBJ) $(ASM_OBJECTS_D) $(CPP_OBJECTS_D) $(CRTN_OBJ) $(CPP_INC_HEADERS)
	rm kernel.elf || true
	$(CC) -T kernel.ld -o kernel.elf $(CRTI_OBJ) $(CRTBEGIN_OBJ) $(ASM_OBJECTS_D) $(CPP_OBJECTS_D) $(CRTEND_OBJ) $(CRTN_OBJ) $(LIB_DEPS) $(LFLAGS) $(LFLAGS_D)

# Clean
.PHONY: clean
clean:
	make -C libapex clean
	make -C libapex++ clean
	rm -f *.ao *.ao_d *.co *.co_d *.crto

# Clean all
.PHONY: distclean
distclean: clean
	make -C libapex distclean
	make -C libapex++ distclean
	rm -f *.elf *.a
	rm -rf $(INC_DIR)
