# Define projects layout
INC_DIR=./include
PROJ_DEPS=libapex libapex++

# Define c++ compiler and Compile/Link FLAGS
CC=i686-elf-g++
AR=i686-elf-ar
IGNORE_WARNINGS=
CFLAGS=-std=c++17 -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti $(foreach warn,$(IGNORE_WARNINGS),-Wno-$(warn)) -I$(INC_DIR) $(foreach proj,$(PROJ_DEPS),-I../$(proj)/include)
CFLAGS_R=-O3
CFLAGS_D=-O0 -g -D_DEBUG
LFLAGS=-ffreestanding -O2 -nostdlib -lgcc
LFLAGS_D=
LIB_DEPS=
ARFLAGS=rcs
ARFLAGS_R=
ARFLAGS_D=

# Define c++ source, header, and object files
CPP_SOURCES=$(wildcard *.cpp)
CPP_HEADERS=$(wildcard *.hpp)
CPP_INC_HEADERS=$(addprefix $(INC_DIR)/, $(basename $(CPP_HEADERS)))
CPP_OBJECTS=$(CPP_SOURCES:.cpp=.co)
CPP_OBJECTS_D=$(CPP_SOURCES:.cpp=.co_d)

# Target list
.PHONY: list
list:
	@echo 'target(deps)       : Description'
	@echo
	@echo 'libio-r        : Builds the common IO framework'
	@echo 'libio-d        : Builds the common IO framework in debug'
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

# Build any .co_d file from it's .cpp file, but add debug flags
%.co_d : %.cpp $(CPP_INC_HEADERS)
	$(CC) -c $< -o $@ $(CFLAGS) $(CFLAGS_D)

# Build the IO framework
libio-r: $(CPP_OBJECTS) $(CPP_INC_HEADERS)
	rm libscreen.a || true
	$(AR) $(ARFLAGS) $(ARFLAGS_R) libio.a $^

libio-d: $(CPP_OBJECTS_D) $(CPP_INC_HEADERS)
	rm libscreen.a || true
	$(AR) $(ARFLAGS) $(ARFLAGS_D) libio.a $^

# Clean
.PHONY: clean
clean:
	rm -f *.ao *.ao_d *.co *.co_d

# Clean all
.PHONY: distclean
distclean: clean
	rm -f *.elf *.a
	rm -rf $(INC_DIR)
