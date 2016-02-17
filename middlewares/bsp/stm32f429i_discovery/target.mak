TARGET = stm32f429i_discovery_bsp

# List C source files here.
CCSOURCES = stm32f429i_discovery.c \
			stm32f429i_discovery_eeprom.c \
			stm32f429i_discovery_gyroscope.c \
			stm32f429i_discovery_io.c \
			stm32f429i_discovery_lcd.c \
			stm32f429i_discovery_sdram.c \
			stm32f429i_discovery_ts.c \
			stmpe811.c \
			ili9341.c \
			l3gd20.c \
			fonts/font12.c \
			fonts/font8.c \
			fonts/font16.c \
			fonts/font20.c \
			fonts/font24.c

# List C++ source files here.
CXXSOURCES =

# List Files to be assembled here
ASOURCES =

# Additional include paths to consider
INCLUDES =	$(ROOT)/middlewares/bsp/stm32f4_discovery

# Folder for sourcecode
SRCDIR = .

include $(ROOT)/build/targets/middleware.mak
