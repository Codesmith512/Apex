# Define c++ compiler and Compile/Link FLAGS
CC=i686-elf-g++
CFLAGS=-ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -I. -Ilibapex
CFLAGS_D=-g
LFLAGS=-ffreestanding -O2 -nostdlib -lgcc
LFLAGS_D=

# Define c++ source, header, and object files
CPP_SOURCES=$(wildcard *.cpp)
CPP_HEADERS=$(wildcard *.hpp)
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
	@echo 'kernel-r(libapex)  : Builds the kernel'
	@echo 'kernel-d(libapex)  : Builds the kernel with debug symbols'
	@echo


# Build any .co file from it's .cpp file
%.co : %.cpp $(CPP_HEADERS)
	$(CC) -c $< -o $@ $(CFLAGS)

# Build any .ao file from it's .asm file
%.ao : %.asm
	$(NASM) $(NASM_FLAGS) -o $@ $<

# Build any .co_d file from it's .cpp file, but add debug flags
%.co_d : %.cpp $(CPP_HEADERS)
	$(CC) -c $< -o $@ $(CFLAGS) $(CFLAGS_D)

# Build any .ao_d file from it's .asm file, but add debug flags
%.ao_d : %.asm
	$(NASM) $(NASM_FLAGS) $(NASM_FLAGS_D) -o $@ $<

# Build the kernel
kernel-r: libapex-r $(ASM_OBJECTS) $(CPP_OBJECTS)
	rm kernel.eld || true
	$(CC) -T kernel.ld -o kernel.elf $(ASM_OBJECTS) $(CPP_OBJECTS) ./libapex/libapex.a $(LFLAGS)

kernel-d: libapex-d $(ASM_OBJECTS_D) $(CPP_OBJECTS_D)
	rm kernel.eld || true
	$(CC) -T kernel.ld -o kernel.elf $(ASM_OBJECTS_D) $(CPP_OBJECTS_D) ./libapex/libapex.a $(LFLAGS) $(LFLAGS_D)

#Build the common system framework
.PHONY: libapex-r
libapex-r:
	make -C libapex libapex-r

.PHONY: libapex-d
libapex-d:
	make -C libapex libapex-d

# Clean
.PHONY: clean
clean:
	make -C libapex clean
	rm -f *.ao *.ao_d *.co *.co_d

# Clean all
.PHONY: distclean
distclean: clean
	make -C libapex distclean
	rm -f *.elf *.a
