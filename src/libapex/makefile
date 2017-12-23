# Define c++ compiler and Compile/Link FLAGS
CC=i686-elf-g++
AR=i686-elf-ar
CFLAGS=-ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -Wno-unused-variable -I.
CFLAGS_R=-O3
CFLAGS_D=-O0 -g
LFLAGS=-ffreestanding -O2 -nostdlib -lgcc
LFLAGS_R=
LFLAGS_D=
ARFLAGS=rcs
ARFLAGS_R=
ARFLAGS_D=

# Define c++ source, header, and object files
CPP_SOURCES=$(wildcard *.cpp)
CPP_HEADERS=$(wildcard *.hpp)
CPP_OBJECTS=$(CPP_SOURCES:.cpp=.co)
CPP_OBJECTS_D=$(CPP_SOURCES:.cpp=.co_d)

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

# Build any .co file from it's .cpp file
%.co : %.cpp $(CPP_HEADERS)
	$(CC) -c $< -o $@ $(CFLAGS) $(CFLAGS_R)

# Build any .co_d file from it's .cpp file, but add debug flags
%.co_d : %.cpp $(CPP_HEADERS)
	$(CC) -c $< -o $@ $(CFLAGS) $(CFLAGS_D)

# Build the STL
libapex-r: $(CPP_OBJECTS)
	rm libapex.a || true
	$(AR) $(ARFLAGS) $(ARFLAGS_R) libapex.a $^

libapex-d: $(CPP_OBJECTS_D)
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
