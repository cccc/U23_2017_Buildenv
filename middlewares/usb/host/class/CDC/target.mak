TARGET = usb_host_class_cdc

# List C source files here.
CCSOURCES = usbh_cdc.c

# List C++ source files here.
CXXSOURCES =

# List Files to be assembled here
ASOURCES =

# Additional include paths to consider
INCLUDES =	$(ROOT)/middlewares/usb/host/class/CDC/Inc

# Folder for sourcecode
SRCDIR = Src

include $(ROOT)/build/targets/middleware.mak
