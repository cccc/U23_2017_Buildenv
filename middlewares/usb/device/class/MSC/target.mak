TARGET = usb_device_class_msc

# List C source files here.
CCSOURCES = usbd_msc.c usbd_msc_bot.c usbd_msc_data.c usbd_msc_scsi.c

# List C++ source files here.
CXXSOURCES =

# List Files to be assembled here
ASOURCES =

# Additional include paths to consider
INCLUDES =	$(ROOT)/middlewares/usb/device/class/MSC/Inc

# Folder for sourcecode
SRCDIR = Src

include $(ROOT)/build/targets/middleware.mak
