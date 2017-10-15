#ifndef _LORA_BONE_H__
#define _LORA_BONE_H__

#include <stdint.h>
#include <stdbool.h>
#include "stm32f1xx_hal.h"
#include "cmsis_gcc.h"

#include "lmic.h"
#include "oslmic.h"

// Handle for SPI1
// Use this Handle after calling bone_initSpi1()
// To drive CS1, use bone_drive_CS1()
extern SPI_HandleTypeDef hspi1;

// Handle for Timer 2
// Timer 2 is used by lmic
extern TIM_HandleTypeDef htim2;

// Handle for UART 1(using USART1 peripheral)
extern UART_HandleTypeDef huart1;

// Initialize UART1 for use with lora-bone
// Baudrate is configured as 115200 Baud
void bone_initUart1();

// Initialize SPI1 for use with lora-bone
// SPI Clock will be 9 MHz
// NSS Pins will be driven in Software
void bone_initSpi1();

// Initialize TIM2 for use with ibm-lmic
void bone_initTim2();

// Initialize lora-bone bsp
// This initializes GPIOC Pin 13 for LED,
void bone_init();

void bone_drive_CS1(bool value);

void bone_set_led(bool value);
void bone_toggle_led();

// Used interal in this BSP
extern uint32_t lmic_time;

#endif

