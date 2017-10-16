#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "stm32f1xx_hal.h"

#include "lmic.h"
#include "lora-bone.h"

static void SystemClock_Config(void);
static void Error_Handler(void);

static const char *hello_world_str = "Hello World!";

//////////////////////////////////////////////////
// MAIN - INITIALIZATION AND STARTUP
//////////////////////////////////////////////////

// initial job
static void initfunc (osjob_t* j) {
	// intialize sensor hardware
	//initsensor();
	// reset MAC state
	LMIC_reset();
	// start joining
	LMIC_startJoining();
	// init done - onEvent() callback will be invoked...
}

/**
 * @brief  Main program
 * @param  None
 * @retval None
 */
int main(int argc, char const *argv[])
{
	osjob_t initjob;

	HAL_Init();
	SystemClock_Config();

	bone_init();
	bone_initUart1();

	printf("Init LoRaWan Mac\n");

	os_init();

	os_setCallback(&initjob, initfunc);
	// execute scheduled jobs and events
	printf("Going into LoRaWan Macs main loop\n");
	os_runloop();
	// (not reached)

	while(1)
	{
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);

		HAL_Delay(500);
	}

	return 0;
}

//////////////////////////////////////////////////
// UTILITY JOB
//////////////////////////////////////////////////

static osjob_t reportjob;

// report sensor value every minute
static void reportfunc (osjob_t* j) {
	bone_set_led(true);
	strncpy((char*)LMIC.frame, hello_world_str,64);
	LMIC_setTxData2(1, LMIC.frame, strlen(hello_world_str), 0); // (port 1, 2 bytes, unconfirmed)
	// reschedule job in 60 seconds
	os_setTimedCallback(j, os_getTime()+sec2osticks(60), reportfunc);
}


//////////////////////////////////////////////////
// LMIC EVENT CALLBACK
//////////////////////////////////////////////////

void onEvent (ev_t ev) {
	//debug_event(ev);

	switch(ev) {
		case EV_JOINING:
			printf("Start Joining\n");
			break;
		// network joined, session established
		case EV_JOINED:
			printf("Joined\n");
			reportfunc(&reportjob);
			break;
		case EV_TXSTART:
			printf("Start Send\n");
			break;
		case EV_TXCOMPLETE:
			printf("Send Complete\n");
			bone_set_led(false);
			break;
		case EV_RXCOMPLETE:
			printf("Receive Complete\n");
			break;
	}
}

/**
 * @brief  System Clock Configuration
 *         The system Clock is configured as follow :
 *            System Clock source            = PLL (HSE)
 *            SYSCLK(Hz)                     = 72000000
 *            HCLK(Hz)                       = 72000000
 *            AHB Prescaler                  = 1
 *            APB1 Prescaler                 = 2
 *            APB2 Prescaler                 = 1
 *            HSE Frequency(Hz)              = 8000000
 *            HSE PREDIV                     = 1
 *            PLLMUL                         = RCC_PLL_MUL9 (9)
 *            Flash Latency(WS)              = 2
 * @param  None
 * @retval None
 */
void SystemClock_Config(void)
{
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;

	/* Enable HSE Oscillator and activate PLL with HSE as source */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
	   clocks dividers */
	RCC_ClkInitStruct.ClockType =
		(RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @param  None
 * @retval None
 */
static void Error_Handler(void)
{
	/* Turn LED5 on */
	while(1)
	{
	}
}

#ifdef  USE_FULL_ASSERT

/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line)
{ 
	/* User can add his own implementation to report the file name and line number,
ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while (1)
	{
	}
}
#endif


