TARGET = stm32f4_discovery_bsp

# List C source files here.
CCSOURCES = stm32f4_discovery.c stm32f4_discovery_accelerometer.c lis302dl.c lis3dsh.c

# List C++ source files here.
CXXSOURCES =

# List Files to be assembled here
ASOURCES =

# Additional include paths to consider
INCLUDES =	$(ROOT)/middlewares/bsp/stm32f4_discovery

# Folder for sourcecode
SRCDIR = .

include $(ROOT)/build/targets/middleware.mak
