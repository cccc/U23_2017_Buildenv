#include <stdint.h>
#include "stm32f1xx_hal.h"
#include "cmsis_gcc.h"


#include "lmic.h"
#include "oslmic.h"

void UartSendStr(char *str);

SPI_HandleTypeDef hspi1;
TIM_HandleTypeDef htim2;

static uint32_t lmic_time = 0;

/*
 * initialize hardware (IO, SPI, TIMER, IRQ).
 */
void hal_init (void)
{
	GPIO_InitTypeDef IntPin, SpiPin, NssPin;

	IntPin.Pin	= GPIO_PIN_3;
	IntPin.Pull	= GPIO_NOPULL;
	IntPin.Speed	= GPIO_SPEED_HIGH;
	IntPin.Mode	= GPIO_MODE_IT_RISING;

	SpiPin.Pin	= GPIO_PIN_5;
	SpiPin.Pull	= GPIO_NOPULL;
	SpiPin.Mode	= GPIO_MODE_AF_PP;
	SpiPin.Speed	= GPIO_SPEED_HIGH;

	NssPin.Pin	= GPIO_PIN_4;
	NssPin.Pull	= GPIO_NOPULL;
	NssPin.Mode	= GPIO_MODE_OUTPUT_PP;
	NssPin.Speed	= GPIO_SPEED_HIGH;

	// Enable clock for the used GPIOs
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	// SPI1 uses at default PA5 for SCK, PA6 for MISO and PA7 for MOSI
	HAL_GPIO_Init(GPIOA, &SpiPin);
	SpiPin.Pin = GPIO_PIN_6;
	HAL_GPIO_Init(GPIOA, &SpiPin);
	SpiPin.Pin = GPIO_PIN_7;
	HAL_GPIO_Init(GPIOA, &SpiPin);

	// NSS Pin is PA4
	HAL_GPIO_Init(GPIOA, &NssPin);

	// DIO0 is PB3, DIO1 is PB4 and DO2 is PB5
	HAL_GPIO_Init(GPIOB, &IntPin);
	IntPin.Pin = GPIO_PIN_4;
	HAL_GPIO_Init(GPIOB, &IntPin);
	IntPin.Pin = GPIO_PIN_5;
	HAL_GPIO_Init(GPIOB, &IntPin);

	// PB3 and PB4 are shared with JTAG
	// JTAG must be disabled
	__HAL_AFIO_REMAP_SWJ_NOJTAG();

	// Enable Interrupts for DIOx
	HAL_NVIC_EnableIRQ(EXTI3_IRQn);
	HAL_NVIC_EnableIRQ(EXTI4_IRQn);
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

	__HAL_RCC_SPI1_CLK_ENABLE();

	hspi1.Instance			= SPI1;
	// SPI1 is part of the APB2 clock domain with 72 MHz.
	// The maximum spi clock for the RFM95W is 10 MHz.
	// A Baurateprescaler of 8 results in a spi clock of 9 MHz.
	hspi1.Init.BaudRatePrescaler	= SPI_BAUDRATEPRESCALER_8;
	hspi1.Init.Direction		= SPI_DIRECTION_2LINES;
	hspi1.Init.DataSize		= SPI_DATASIZE_8BIT;
	hspi1.Init.CRCCalculation	= SPI_CRCCALCULATION_DISABLED;
	hspi1.Init.CLKPhase		= SPI_PHASE_1EDGE;
	hspi1.Init.CLKPolarity		= SPI_POLARITY_LOW;
	hspi1.Init.Mode			= SPI_MODE_MASTER;
	hspi1.Init.NSS			= SPI_NSS_SOFT;
	hspi1.Init.TIMode		= SPI_TIMODE_DISABLED;
	hspi1.Init.FirstBit		= SPI_FIRSTBIT_MSB;
	
	HAL_SPI_Init(&hspi1);

	__HAL_RCC_TIM2_CLK_ENABLE();
	htim2.Instance			= TIM2;
	htim2.Init.ClockDivision	= TIM_CLOCKDIVISION_DIV1;
	// TIM2 is part of the APB1 clockdomain with 36 MHz.
	// But the base clock of TIM2 gets multiplied by 2 if the APB1 Prescaler is not 1.
	// With an clock prescaler of 1 this results in an base clock of TIM2 at SystemCoreClock(72 MHz).
	// TIM2 will count up with OSTICKS_PER_SEC(defined in oslmic.h) and the interrupt
	// will be generated every second.
	htim2.Init.Prescaler		= (SystemCoreClock/OSTICKS_PER_SEC)-1; 
	htim2.Init.Period		= OSTICKS_PER_SEC-1;
	htim2.Init.RepetitionCounter	= 0;
	htim2.Init.CounterMode		= TIM_COUNTERMODE_UP;

	HAL_TIM_Base_Init(&htim2);

	HAL_NVIC_EnableIRQ(TIM2_IRQn);

	HAL_TIM_Base_Start_IT(&htim2);
}

void TIM2_IRQHandler()
{
	if (__HAL_TIM_GET_FLAG(&htim2,TIM_FLAG_UPDATE) != RESET && __HAL_TIM_GET_IT_SOURCE(&htim2,TIM_IT_UPDATE) != RESET)
	{
		__HAL_TIM_CLEAR_IT(&htim2,TIM_IT_UPDATE);
		lmic_time += OSTICKS_PER_SEC;
	}
}

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

/*
 * drive radio NSS pin (0=low, 1=high).
 */
void hal_pin_nss (u1_t val)
{
	// NSS Pin is PA4
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,(val==0)?GPIO_PIN_RESET:GPIO_PIN_SET);
}

/*
 * drive radio RX/TX pins (0=rx, 1=tx).
 */
void hal_pin_rxtx (u1_t val)
{
	// Nothing to do here
	// Our board has only one antenna, no need to switch between rx and tx antenna
}

/*
 * control radio RST pin (0=low, 1=high, 2=floating)
 */
void hal_pin_rst (u1_t val)
{
	// Nothing to do here
	// Reset pin is not connected. It can be connected by soldering Jumper J13.
	// Reset will be connected to PB12
}

/*
 * perform 8-bit SPI transaction with radio.
 *   - write given byte 'outval'
 *   - read byte and return value
 */
u1_t hal_spi (u1_t outval)
{
	uint8_t result;
	HAL_SPI_TransmitReceive(&hspi1,&outval,&result,1,9999);
	return result;
}


static unsigned int  irq_disable_cnt = 0;
/*
 * disable all CPU interrupts.
 *   - might be invoked nested 
 *   - will be followed by matching call to hal_enableIRQs()
 */
void hal_disableIRQs (void)
{
	if (irq_disable_cnt++ == 0)
		__disable_irq();
}

/*
 * enable CPU interrupts.
 */
void hal_enableIRQs (void)
{
	if (irq_disable_cnt > 0 && --irq_disable_cnt == 0)
		__enable_irq();
}

/*
 * put system and CPU in low-power mode, sleep until interrupt.
 */
void hal_sleep (void)
{
	// TODO: Implement Powermanagement	
}

/*
 * return 32-bit system time in ticks.
 */
u4_t hal_ticks (void)
{
	return lmic_time+__HAL_TIM_GET_COUNTER(&htim2);
}

/*
 * busy-wait until specified timestamp (in ticks) is reached.
 */
void hal_waitUntil (u4_t time)
{
	uint32_t targettime = time-lmic_time;
	while (__HAL_TIM_GET_COUNTER(&htim2)<targettime);
}

/*
 * check and rewind timer for target time.
 *   - return 1 if target time is close
 *   - otherwise rewind timer for target time or full period and return 0
 */
u1_t hal_checkTimer (u4_t targettime)
{
	if (targettime <= hal_ticks())
		return 1;
	return 0;
}

/*
 * perform fatal failure action.
 *   - called by assertions
 *   - action could be HALT or reboot
 */
void hal_failed (void)
{
	UartSendStr("LMIC Hal failed");
	while(1);
}

