This folder contains different linker scripts.

The F4-Scripts were harvested with the following command line from the STM32F4-Cube distribution as these files are scattered around in different projects there
	find . -name "STM32F4*.ld" -exec cp {} /home/andy/Desktop/maple/misc/linker/f4 \;
There may be duplicates that I didn't remove. For example STM32F407VG_FLASH.ld and STM32F407VGTx_FLASH.ld look like they are the same

The F1-Scripts are taken from /STM32Cube_FW_F1_V1.2.0/Drivers/CMSIS/Device/ST/STM32F1xx/Source/Templates/gcc/linker/
The F1-Scripts had a weird '0'-character on each newline. Replaced "\0\n" with "\n".
