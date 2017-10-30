# Target file name.
TARGET = blinky

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
LDFLAGS := -lc -Wl,--gc-sections
# Uncomment this to enable semihosting
#LDFLAGS += -specs=rdimon.specs -lrdimon
# Comment this when enable semihosting
LDFLAGS += -specs=nosys.specs
LDFLAGS += -mthumb -mcpu=cortex-m3
LDFLAGS += -Wl,-T$(ROOT)/misc/linker/f1/STM32F103XB_FLASH.ld,-Map,$(SELF_DIR)/$(TARGET).map

# Additional include paths to consider
INCLUDES = $(SELF_DIR)/inc

# Middlewares to add
# source files are built with this target (specific for this target, so local header files are consideres)
# include paths are added automatically
MIDDLEWARES = cmsis stm32f1_device_103xb stm32f1_hal lora-bone_bsp

# Additional local static libs to link against
LIBS = 

# Folder for object files
OBJDIR = obj

# Folder for sourcecode
SRCDIR = src

# Additional defines
DEFINES := -DSTM32F103xB -DUSE_HAL_DRIVER -DNO_LMIC_SUPPORT

include $(ROOT)/build/targets/executable.mak
