# Target file name.
TARGET = libdummy

# List C source files here.
CCSOURCES =	dummy.c

# List C++ source files here.
CXXSOURCES = 

# List Assembler to be assembled here
ASOURCES = 

# CPU flags
CPUFLAGS := -march=armv7-a -mtune=cortex-a9 -mfpu=neon -ftree-vectorize -ffast-math -mfloat-abi=softfp

# C compiler flags
CFLAGS := -std=gnu99 -ggdb -O0 $(CPUFLAGS)
CFLAGS += -Wall 
CFLAGS += -fdata-sections -ffunction-sections

# C++ compiler flags
CXXFLAGS :=

# GAS flags
ASFLAGS := $(CPUFLAGS)
ASFLAGS += -fdata-sections -ffunction-sections

# Additional include paths to consider
INCLUDES = $(ROOT)/libs/dummy/inc

# Additional local static libs to link against
LIBS = 

# Folder for object files
OBJDIR = obj

# Folder for sourcecode
SRCDIR = src

# Additional defines
DEFINES := 

include $(ROOT)/build/targets/lib.mak
