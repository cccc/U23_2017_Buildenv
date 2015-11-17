/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAPLE_MINI_H
#define __MAPLE_MINI_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup MAPLE_MINI
  * @{
  */ 
  
/** @defgroup MAPLE_MINI_Exported_Types Exported_Types
  * @{
  */
typedef enum 
{
  LED1 = 0,

  LED_BLUE  = LED1
    
} Led_TypeDef;

typedef enum 
{
  BUTTON_USER = 0,
} Button_TypeDef;

typedef enum 
{  
  BUTTON_MODE_GPIO = 0,
  BUTTON_MODE_EXTI = 1
} ButtonMode_TypeDef;
/**
  * @}
  */ 

/** @defgroup MAPLE_MINI_Exported_Constants Exported_Constants
  * @{
  */ 

/** 
  * @brief  Define for Maple Mini board  
  */ 
#if !defined (USE_MAPLE_MINI)
 #define USE_MAPLE_MINI
#endif
  
/** @defgroup MAPLE_MINI_LED LED constants
  * @{
  */
#define LEDn                             1

#define LED1_PIN                         GPIO_PIN_1           /* PB.01 */
#define LED1_GPIO_PORT                   GPIOB
#define LED1_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOB_CLK_ENABLE()
#define LED1_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOB_CLK_DISABLE()

#define LEDx_GPIO_CLK_ENABLE(__INDEX__)   do {LED1_GPIO_CLK_ENABLE();} while(0)

#define LEDx_GPIO_CLK_DISABLE(__INDEX__)  (LED1_GPIO_CLK_DISABLE())
/**
  * @}
  */
  
/** @defgroup MAPLE_MINI_BUTTON Buttons constants
  * @{
  */  
#define BUTTONn                          1
/**
 * @brief Key push-button
 */
#define USER_BUTTON_PIN                  GPIO_PIN_8           /* PB.08 */
#define USER_BUTTON_GPIO_PORT            GPIOB
#define USER_BUTTON_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOB_CLK_ENABLE()
#define USER_BUTTON_GPIO_CLK_DISABLE()   __HAL_RCC_GPIOB_CLK_DISABLE()
#define USER_BUTTON_EXTI_IRQn            EXTI9_5_IRQn

#define BUTTONx_GPIO_CLK_ENABLE(__INDEX__)     do {USER_BUTTON_GPIO_CLK_ENABLE();} while(0)

#define BUTTONx_GPIO_CLK_DISABLE(__INDEX__)    (USER_BUTTON_GPIO_CLK_DISABLE())

/**
  * @}
  */ 

/**
  * @}
  */ 
  
/** @addtogroup MAPLE_MINI_Exported_Functions
  * @{
  */ 
uint32_t  BSP_GetVersion(void);
void      BSP_LED_Init(Led_TypeDef Led);
void      BSP_LED_On(Led_TypeDef Led);
void      BSP_LED_Off(Led_TypeDef Led);
void      BSP_LED_Toggle(Led_TypeDef Led);
void      BSP_PB_Init(Button_TypeDef Button, ButtonMode_TypeDef Mode);
uint32_t  BSP_PB_GetState(Button_TypeDef Button);
/**
  * @}
  */
/**
  * @}
  */ 

/**
  * @}
  */ 
  
#ifdef __cplusplus
}
#endif
  
#endif /* __MAPLE_MINI_H */
