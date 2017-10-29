#include "lora-bone.h"

void TIM2_IRQHandler()
{
	if (__HAL_TIM_GET_FLAG(&htim2,TIM_FLAG_UPDATE) != RESET && __HAL_TIM_GET_IT_SOURCE(&htim2,TIM_IT_UPDATE) != RESET)
	{
		__HAL_TIM_CLEAR_IT(&htim2,TIM_IT_UPDATE);
		lmic_time += OSTICKS_PER_SEC;
	}
}

#ifndef NO_LMIC_SUPPORT
// Interrupt Handler for DIO0
void EXTI3_IRQHandler()
{
	//UartSendStr("DIO0");
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_3);
	radio_irq_handler(0);
}

// Interrupt Handler for DIO1
void EXTI4_IRQHandler()
{
	//UartSendStr("DIO1");
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_4);
	radio_irq_handler(1);
}

// Interrupt Handler for DIO2
void EXTI9_5_IRQHandler()
{
	if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_5) != RESET)
	{
		//UartSendStr("DIO2");
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_5);
		radio_irq_handler(2);
	}
}
#endif

