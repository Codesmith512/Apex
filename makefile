# Define c++ compiler and Compile/Link FLAGS
CC=i686-elf-g++
CFLAGS=-ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti
LFLAGS=-ffreestanding -O2 -nostdlib -lgcc

# Define NASM flags
NASM=nasm

# Build the kernel
kernel: boot.asm kernel.cpp
	$(NASM) -f elf32 -o boot.o boot.asm
	$(CC) -c kernel.cpp -o kernel.o $(CFLAGS)
	$(CC) -T kernel.ld -o os.bin boot.o kernel.o $(LFLAGS)

# Clean rule
.PHONY: clean
clean:
	rm -f *.o *.bin