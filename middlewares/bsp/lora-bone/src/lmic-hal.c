#include "lora-bone.h"

uint32_t lmic_time = 0;

/*
 * initialize hardware (IO, SPI, TIMER, IRQ).
 */
void hal_init (void)
{
	// Enable clock for the used GPIOs
	__HAL_RCC_GPIOB_CLK_ENABLE();

	// DIO0 is PB3, DIO1 is PB4 and DO2 is PB5
	// Configure them as external interrupt on the rising edge
	HAL_GPIO_Init(GPIOB, &(GPIO_InitTypeDef){
		.Pin	= GPIO_PIN_3,
		.Pull	= GPIO_NOPULL,
		.Speed	= GPIO_SPEED_HIGH,
		.Mode	= GPIO_MODE_IT_RISING
	});
	HAL_GPIO_Init(GPIOB, &(GPIO_InitTypeDef){
		.Pin	= GPIO_PIN_4,
		.Pull	= GPIO_NOPULL,
		.Speed	= GPIO_SPEED_HIGH,
		.Mode	= GPIO_MODE_IT_RISING
	});
	HAL_GPIO_Init(GPIOB, &(GPIO_InitTypeDef){
		.Pin	= GPIO_PIN_5,
		.Pull	= GPIO_NOPULL,
		.Speed	= GPIO_SPEED_HIGH,
		.Mode	= GPIO_MODE_IT_RISING
	});
	
	// PB3 and PB4 are shared with JTAG
	// JTAG must be disabled
	__HAL_AFIO_REMAP_SWJ_NOJTAG();

	// Enable Interrupts for DIOx
	HAL_NVIC_EnableIRQ(EXTI3_IRQn);
	HAL_NVIC_EnableIRQ(EXTI4_IRQn);
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

	// The RFM95W Module is connected at SPI1 so we should initialize it
	bone_initSpi1();
	// The lmic library needs an Time Reference which even works when interrupts are disabled
	bone_initTim2();
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
	printf("LMIC Hal failed\n");
	while(1);
}

