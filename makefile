# Compilers
CC      := g++

# Dirs
SRC     := .
LIBDIR  := lib

# Sources
SRCS1    := $(SRC)/make_stereogram.cpp

SRCS2    := $(SRC)/make_gif.cpp

# Flags
COMMON_FLAGS := -Wall -Wextra -Wno-missing-field-initializers


# Default target
all: make_stereogram make_gif

# Native build
make_stereogram: $(SRCS1)
	$(CC) $(COMMON_FLAGS) -I $(LIBDIR) -o $@ $^

make_gif: $(SRCS2)
	$(CC) $(COMMON_FLAGS) -I $(LIBDIR) -o $@ $^
