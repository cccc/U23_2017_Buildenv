TARGET = usb_device_class_customhid

# List C source files here.
CCSOURCES = usbd_customhid.c

# List C++ source files here.
CXXSOURCES =

# List Files to be assembled here
ASOURCES =

# Additional include paths to consider
INCLUDES =	$(ROOT)/middlewares/usb/device/class/CustomHID/Inc

# Folder for sourcecode
SRCDIR = Src

include $(ROOT)/build/targets/middleware.mak
