# Target file name.
TARGET = ibm-lmic

# List C source files here.
CCSOURCES = aes.c lmic.c oslmic.c radio.c

# List C++ source files here.
CXXSOURCES = 

# List Assembler to be assembled here
ASOURCES = 

# CPU flags
CPUFLAGS := -mthumb -mcpu=cortex-m3 -msoft-float -mfloat-abi=soft

# C compiler flags
CFLAGS := -std=gnu99 -ggdb -O0 $(CPUFLAGS)
CFLAGS += -Wall 
CFLAGS += -fdata-sections -ffunction-sections
CFLAGS += -DCFG_eu868 -DCFG_sx1276_radio

# C++ compiler flags
CXXFLAGS :=

# GAS flags
ASFLAGS := $(CPUFLAGS)
ASFLAGS += -fdata-sections -ffunction-sections

# Additional include paths to consider
INCLUDES = $(ROOT)/libs/ibm-lmic/inc

# Additional local static libs to link against
LIBS = 

# Folder for object files
OBJDIR = obj

# Folder for sourcecode
SRCDIR = src

# Additional defines
DEFINES := 

include $(ROOT)/build/targets/lib.mak
