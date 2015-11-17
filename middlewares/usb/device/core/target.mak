TARGET = usb_device_core

# List C source files here.
CCSOURCES = usbd_core.c usbd_ctlreq.c usbd_ioreq.c

# List C++ source files here.
CXXSOURCES =

# List Files to be assembled here
ASOURCES =

# Additional include paths to consider
INCLUDES =	$(ROOT)/middlewares/usb/device/core/Inc

# Folder for sourcecode
SRCDIR = Src

include $(ROOT)/build/targets/middleware.mak
