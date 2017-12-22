# Define c++ compiler and Compile/Link FLAGS
CC=i686-elf-g++
CFLAGS=-ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -Wunused-variable -I.
LFLAGS=-ffreestanding -O2 -nostdlib -lgcc
AR=i686-elf-ar
ARFLAGS=rcs

# Define c++ source, header, and object files
CPP_SOURCES=$(wildcard *.cpp)
CPP_HEADERS=$(wildcard *.hpp)
CPP_OBJECTS=$(CPP_SOURCES:.cpp=.co)

# Target list
.PHONY: list
list:
	@echo Valid targets:
	@echo libapex : Builds the STL
	@echo clean : Cleans up all intermediate files
	@echo distclean : Cleans up all everything

# Build any .cpp.o file from it's .cpp file
%.co : %.cpp $(CPP_HEADERS)
	$(CC) -c $< -o $@ $(CFLAGS)

# Build the STL
libapex: $(CPP_OBJECTS)
	$(AR) $(ARFLAGS) $@.a $^

# Clean rule
.PHONY: clean
clean:
	rm -f *.ao *.co

# Cleans all virtual device targets
.PHONY: distclean
distclean: clean
	rm -f *.elf *.a
