TARGET = stm32vl_discovery_bsp

# List C source files here.
CCSOURCES = stm32vl_discovery.c

# List C++ source files here.
CXXSOURCES =

# List Files to be assembled here
ASOURCES =

# Additional include paths to consider
INCLUDES =	$(ROOT)/middlewares/bsp/stm32vl_discovery

# Folder for sourcecode
SRCDIR = .

include $(ROOT)/build/targets/middleware.mak
