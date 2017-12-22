# Define c++ compiler and Compile/Link FLAGS
CC=i686-elf-g++
CFLAGS=-ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -I. -Ilibapex
LFLAGS=-ffreestanding -O2 -nostdlib -lgcc

# Define c++ source, header, and object files
CPP_SOURCES=$(wildcard *.cpp)
CPP_HEADERS=$(wildcard *.hpp)
CPP_OBJECTS=$(CPP_SOURCES:.cpp=.co)

# Define NASM flags
NASM=nasm

# Define asm source and object files
ASM_SOURCES=$(wildcard *.asm)
ASM_OBJECTS=$(ASM_SOURCES:.asm=.ao)

# Target list
.PHONY: list
list:
	@echo Valid targets:
	@echo kernel : Builds the kernel
	@echo libapex : Builds the common system library
	@echo clean : Cleans up all intermediate files
	@echo dist-clean : Cleans up all everything

# Build any .cpp.o file from it's .cpp file
%.co : %.cpp $(CPP_HEADERS)
	$(CC) -c $< -o $@ $(CFLAGS)

# Build any .asm.o file from it's .asm file
%.ao : %.asm
	$(NASM) -f elf32 -o $@ $<

#Build the STL
.PHONY: libapex
libapex:
	make -C libapex libapex

# Build the kernel
kernel: $(ASM_OBJECTS) $(CPP_OBJECTS) libapex
	$(CC) -T kernel.ld -o $@.elf $(ASM_OBJECTS) $(CPP_OBJECTS) ./libapex/libapex.a $(LFLAGS)

# Clean rule
.PHONY: clean
clean:
	make -C libapex clean
	rm -f *.ao *.co

# Cleans all virtual device targets
.PHONY: distclean
distclean: clean
	make -C libapex distclean
	rm -f *.elf *.a
