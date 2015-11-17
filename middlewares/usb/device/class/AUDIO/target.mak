TARGET = usb_device_class_audio

# List C source files here.
CCSOURCES = usbd_audio.c

# List C++ source files here.
CXXSOURCES =

# List Files to be assembled here
ASOURCES =

# Additional include paths to consider
INCLUDES =	$(ROOT)/middlewares/usb/device/class/AUDIO/Inc

# Folder for sourcecode
SRCDIR = Src

include $(ROOT)/build/targets/middleware.mak
