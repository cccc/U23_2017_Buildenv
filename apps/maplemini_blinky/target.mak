# Target file name.
TARGET = maplemini_blinky

# List C source files here.
CCSOURCES = main.c interrupts.c

# List C++ source files here.
CXXSOURCES =

# List Files to be assembled here
ASOURCES = 

# C compiler flags
CFLAGS  = -std=gnu99 -ggdb -O0 -Werror -mthumb -mcpu=cortex-m3
CFLAGS += -fdata-sections -ffunction-sections

# C++ compiler flags
CXXFLAGS =

# GAS flags
ASFLAGS = -fdata-sections -ffunction-sections

# Linker flags
LDFLAGS := -lnosys -lc -Wl,--gc-sections
LDFLAGS += -mthumb -mcpu=cortex-m3
LDFLAGS += -Wl,-T$(ROOT)/misc/linker/f1/STM32F103XB_FLASH.ld,-Map,$(ROOT)/apps/$(TARGET)/$(TARGET).map

# Additional include paths to consider
INCLUDES = $(ROOT)/apps/$(TARGET)/inc \
           $(ROOT)/libs/libdummy/inc

# Middlewares to add
# source files are built with this target (specific for this target, so local header files are consideres)
# include paths are added automatically
MIDDLEWARES = cmsis stm32f1_device_103xb stm32f1_hal maple_mini_bsp

# Additional local static libs to link against
LIBS = $(BINARY-libdummy)

# Folder for object files
OBJDIR = obj

# Folder for sourcecode
SRCDIR = src

# Additional defines
DEFINES := -DSTM32F103xB -DUSE_HAL_DRIVER

include $(ROOT)/build/targets/executable.mak
