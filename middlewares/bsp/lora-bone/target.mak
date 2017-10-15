TARGET = lora-bone_bsp

# List C source files here.
CCSOURCES = lora-bone.c lmic-hal.c bone-interrupts.c

# List C++ source files here.
CXXSOURCES =

# List Files to be assembled here
ASOURCES =

# Additional include paths to consider
INCLUDES =	$(ROOT)/middlewares/bsp/lora-bone/inc

# Folder for sourcecode
SRCDIR = src

include $(ROOT)/build/targets/middleware.mak
