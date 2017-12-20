# Define c++ compiler and Compile/Link FLAGS
CC=i686-elf-g++
CFLAGS=-ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti
LFLAGS=-ffreestanding -O2 -nostdlib -lgcc

# Define c++ source, header, and object files
CPP_SOURCES=$(wildcard *.cpp)
CPP_HEADERS=$(wildcard *.h)

# Define NASM flags
NASM=nasm

# Define asm source and object files
ASM_SOURCES=$(wildcard *.asm)

# Target list
.PHONY: list
list:
	@echo Valid targets:
	@echo kernel : Builds the kernel
	@echo clean : Cleans up all intermediate files

# Build any .cpp.o file from it's .cpp file
%.co : %.cpp
	$(CC) -c $< -o $@ $(CFLAGS)

# Build any .asm.o file from it's .asm file
%.ao : %.asm
	$(NASM) -f elf32 -o $@ $<

# Build the kernel
kernel: $(ASM_SOURCES:.asm=.ao) $(CPP_SOURCES:.cpp=.co) $(CPP_HEADERS)
	$(CC) -T kernel.ld -o $@.elf $^ $(LFLAGS)

# Build the virtual disk for the kernel
vdisk: kernel
# Create the virtual disk image
	dd if=/dev/zero of=./disk.img bs=1M count=1

# Format disk image
	mkfs.ext2 ./disk.img

# Mount disk image and set ownership
	mkdir disk.img.d
	sudo mount -o loop=/dev/loop0 ./disk.img ./disk.img.d
	sudo chown -R code:code ./disk.img.d

# Copy disk files
	cp -r disk/* disk.img.d/

# Copy kernel
	cp kernel.elf disk.img.d/boot/kernel.elf

# Unmount disk
	sudo umount disk.img.d
	rmdir disk.img.d

# Clean rule
.PHONY: clean
clean:
	rm -f *.ao *.co *.elf *.img *.raw
