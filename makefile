# Compilers
CC      := g++

# Dirs
SRC     := .
LIBDIR  := lib

# Sources
SRCS1    := $(SRC)/shift_pixels.cpp

SRCS2    := $(SRC)/make_gif.cpp

# Flags
COMMON_FLAGS := -Wall -Wextra -Wno-missing-field-initializers


# Default target
all: shift_pixels make_gif

# Native build
shift_pixels: $(SRCS1)
	$(CC) $(COMMON_FLAGS) -I $(LIBDIR) -o $@ $^

make_gif: $(SRCS2)
	$(CC) $(COMMON_FLAGS) -I $(LIBDIR) -o $@ $^
