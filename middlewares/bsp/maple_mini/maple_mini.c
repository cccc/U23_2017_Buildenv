/* Includes ------------------------------------------------------------------*/
#include "maple_mini.h"

/** @addtogroup BSP
  * @{
  */

/** @defgroup MAPLE_MINI Maple Mini
  * @brief This file provides firmware functions to manage Leds and push-buttons
  *        available on Maple Mini board.
  * @{
  */ 

/** @defgroup MAPLE_MINI_Private_Defines Private_Defines
  * @{
  */ 
  
/**
* @brief Maple Mini BSP Driver version number
*/
#define __MAPLE_MINI_BSP_VERSION_MAIN      (0x01) /*!< [31:24] main version */
#define __MAPLE_MINI_BSP_VERSION_SUB1      (0x00) /*!< [23:16] sub1 version */
#define __MAPLE_MINI_BSP_VERSION_SUB2      (0x00) /*!< [15:8]  sub2 version */
#define __MAPLE_MINI_BSP_VERSION_RC        (0x00) /*!< [7:0]  release candidate */ 
#define __MAPLE_MINI_BSP_VERSION           ((__MAPLE_MINI_BSP_VERSION_MAIN << 24)\
                                           |(__MAPLE_MINI_BSP_VERSION_SUB1 << 16)\
                                           |(__MAPLE_MINI_BSP_VERSION_SUB2 << 8 )\
                                           |(__MAPLE_MINI_BSP_VERSION_RC)) 
/**
  * @}
  */ 

/** @defgroup MAPLE_MINI_Private_Variables Private_Variables
  * @{
  */ 
GPIO_TypeDef*   LED_PORT[LEDn] = {LED1_GPIO_PORT};
const uint16_t  LED_PIN[LEDn]  = {LED1_PIN};


GPIO_TypeDef*   BUTTON_PORT[BUTTONn]  = {USER_BUTTON_GPIO_PORT};
const uint16_t  BUTTON_PIN[BUTTONn]   = {USER_BUTTON_PIN};
const uint8_t   BUTTON_IRQn[BUTTONn]  = {USER_BUTTON_EXTI_IRQn};

/**
  * @}
  */ 


/** @defgroup MAPLE_MINI_Exported_Functions Exported_Functions
  * @{
  */ 

/**
  * @brief  This method returns the Maple Mini BSP Driver version
  * @retval version : 0xXYZR (8bits for each decimal, R for RC)
  */
uint32_t BSP_GetVersion(void)
{
  return __MAPLE_MINI_BSP_VERSION;
}

/**
  * @brief  Configures LED GPIO.
  * @param  Led: Specifies the Led to be configured. 
  *   This parameter can be one of following parameters:
  *     @arg LED3
  *     @arg LED4
  * @retval None
  */
void BSP_LED_Init(Led_TypeDef Led)
{
  GPIO_InitTypeDef  gpioinitstruct = {0};
  
  /* Enable the GPIO_LED Clock */
  LEDx_GPIO_CLK_ENABLE(Led);

  /* Configure the GPIO_LED pin */
  gpioinitstruct.Pin    = LED_PIN[Led];
  gpioinitstruct.Mode   = GPIO_MODE_OUTPUT_PP;
  gpioinitstruct.Pull   = GPIO_NOPULL;
  gpioinitstruct.Speed  = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(LED_PORT[Led], &gpioinitstruct);

  /* Reset PIN to switch off the LED */
  HAL_GPIO_WritePin(LED_PORT[Led], LED_PIN[Led], GPIO_PIN_RESET);
}

/**
  * @brief  Turns selected LED On.
  * @param  Led: Specifies the Led to be set on. 
  *   This parameter can be one of following parameters:
  *     @arg LED3
  *     @arg LED4
  * @retval None
  */
void BSP_LED_On(Led_TypeDef Led)
{
  HAL_GPIO_WritePin(LED_PORT[Led], LED_PIN[Led], GPIO_PIN_SET);
}

/**
  * @brief  Turns selected LED Off.
  * @param  Led: Specifies the Led to be set off. 
  *   This parameter can be one of following parameters:
  *     @arg LED3
  *     @arg LED4
  * @retval None
  */
void BSP_LED_Off(Led_TypeDef Led)
{
  HAL_GPIO_WritePin(LED_PORT[Led], LED_PIN[Led], GPIO_PIN_RESET);
}

/**
  * @brief  Toggles the selected LED.
  * @param  Led: Specifies the Led to be toggled. 
  *   This parameter can be one of following parameters:
  *     @arg LED3
  *     @arg LED4
  * @retval None
  */
void BSP_LED_Toggle(Led_TypeDef Led)
{
  HAL_GPIO_TogglePin(LED_PORT[Led], LED_PIN[Led]);
}

/**
  * @brief  Configures Button GPIO and EXTI Line.
  * @param  Button: Specifies the Button to be configured.
  *   This parameter should be: BUTTON_USER
  * @param  Button_Mode: Specifies Button mode.
  *   This parameter can be one of following parameters:   
  *     @arg BUTTON_MODE_GPIO: Button will be used as simple IO 
  *     @arg BUTTON_MODE_EXTI: Button will be connected to EXTI line with interrupt
  *                     generation capability  
  * @retval None
  */
void BSP_PB_Init(Button_TypeDef Button, ButtonMode_TypeDef Button_Mode)
{
  GPIO_InitTypeDef gpioinitstruct = {0};

  /* Enable the BUTTON Clock */
  BUTTONx_GPIO_CLK_ENABLE(Button);

  gpioinitstruct.Pin    = BUTTON_PIN[Button];
  gpioinitstruct.Pull   = GPIO_NOPULL;
  gpioinitstruct.Speed  = GPIO_SPEED_HIGH;

  if (Button_Mode == BUTTON_MODE_GPIO)
  {
    /* Configure Button pin as input */
    gpioinitstruct.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(BUTTON_PORT[Button], &gpioinitstruct);
  }
  else if (Button_Mode == BUTTON_MODE_EXTI)
  {
    /* Configure Button pin as input with External interrupt */
    gpioinitstruct.Mode = GPIO_MODE_IT_RISING;
    HAL_GPIO_Init(BUTTON_PORT[Button], &gpioinitstruct);

    /* Enable and set Button EXTI Interrupt to the lowest priority */
    HAL_NVIC_SetPriority((IRQn_Type)(BUTTON_IRQn[Button]), 0x0F, 0);
    HAL_NVIC_EnableIRQ((IRQn_Type)(BUTTON_IRQn[Button]));
  }
}

/**
  * @brief  Returns the selected Button state.
  * @param  Button: Specifies the Button to be checked.
  *   This parameter should be: BUTTON_USER  
  * @retval Button state.
  */
uint32_t BSP_PB_GetState(Button_TypeDef Button)
{
  return HAL_GPIO_ReadPin(BUTTON_PORT[Button], BUTTON_PIN[Button]);
}

/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */
