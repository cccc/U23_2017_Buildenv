TARGET = usb_host_core

# List C source files here.
CCSOURCES = usbh_core.c usbh_ctlreq.c usbh_ioreq.c usbh_pipes.c

# List C++ source files here.
CXXSOURCES =

# List Files to be assembled here
ASOURCES =

# Additional include paths to consider
INCLUDES =	$(ROOT)/middlewares/usb/host/core/Inc

# Folder for sourcecode
SRCDIR = Src

include $(ROOT)/build/targets/middleware.mak
