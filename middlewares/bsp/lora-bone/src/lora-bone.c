#include "lora-bone.h"

SPI_HandleTypeDef hspi1;
I2C_HandleTypeDef hi2c1;
TIM_HandleTypeDef htim2;
UART_HandleTypeDef huart1;

static bool bone_bsp_initialized = false;

// Initialize UART1 for use with lora-bone
// Baudrate is 115200
void bone_initUart1()
{
	static bool initialized = false;

	// Only do initialization once
	if (initialized)
		return;
	initialized = true;


	// Turn USART1 Clock on
	__HAL_RCC_USART1_CLK_ENABLE();
	// Turn GPIOA Clock on
	__HAL_RCC_GPIOA_CLK_ENABLE();

	// As defaut USART1 uses PINA10 as RX and PINA9 as TX
	HAL_GPIO_Init(GPIOA, &(GPIO_InitTypeDef){
		.Mode	= GPIO_MODE_AF_OD,
		.Pull	= GPIO_PULLUP,
		.Pin	= GPIO_PIN_9,
		.Speed	= GPIO_SPEED_HIGH,

	});
	HAL_GPIO_Init(GPIOA, &(GPIO_InitTypeDef){
		.Mode	= GPIO_MODE_AF_INPUT,
		.Pull	= GPIO_NOPULL,
		.Pin	= GPIO_PIN_10,
		.Speed	= GPIO_SPEED_HIGH,

	});

	huart1.Instance		= USART1;
	huart1.Init.Parity	= UART_PARITY_NONE;
	huart1.Init.WordLength	= UART_WORDLENGTH_8B;
	//huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	huart1.Init.Mode	= UART_MODE_TX;
	huart1.Init.StopBits	= UART_STOPBITS_2;
	huart1.Init.HwFlowCtl	= UART_HWCONTROL_NONE;
	huart1.Init.BaudRate	= 115200;

	// Initialize Uart
	if (HAL_UART_Init(&huart1) != HAL_OK)
	{
		printf("UART failed!\n");
		while(1);
	}
}

// Initialize SPI1 for use with lora-bone
// SPI Clock will be 9 MHz
// NSS Pins will be driven in Software
void bone_initSpi1()
{
	static bool initialized = false;

	// Only do initialization once
	if (initialized)
		return;
	initialized = true;

	// Turn GPIOA Clock on
	__HAL_RCC_GPIOA_CLK_ENABLE();

	// Init CS1, my be used by User for communicating with extra Hardware
	HAL_GPIO_Init(GPIOA, &(GPIO_InitTypeDef){
		.Pin	= GPIO_PIN_3,
		.Mode	= GPIO_MODE_OUTPUT_PP,
		.Pull	= GPIO_NOPULL,
		.Speed	= GPIO_SPEED_HIGH
	});
	// Init CS0, used by lmic for communicating with RFM95W
	HAL_GPIO_Init(GPIOA, &(GPIO_InitTypeDef){
		.Pin	= GPIO_PIN_4,
		.Mode	= GPIO_MODE_OUTPUT_PP,
		.Pull	= GPIO_NOPULL,
		.Speed	= GPIO_SPEED_HIGH
	});
	// Init SCK
	HAL_GPIO_Init(GPIOA, &(GPIO_InitTypeDef){
		.Pin	= GPIO_PIN_5,
		.Mode	= GPIO_MODE_AF_PP,
		.Pull	= GPIO_NOPULL,
		.Speed	= GPIO_SPEED_HIGH
	});
	// Init MISO
	HAL_GPIO_Init(GPIOA, &(GPIO_InitTypeDef){
		.Pin	= GPIO_PIN_6,
		.Mode	= GPIO_MODE_AF_PP,
		.Pull	= GPIO_NOPULL,
		.Speed	= GPIO_SPEED_HIGH
	});
	// Init MOSI
	HAL_GPIO_Init(GPIOA, &(GPIO_InitTypeDef){
		.Pin	= GPIO_PIN_7,
		.Mode	= GPIO_MODE_AF_PP,
		.Pull	= GPIO_NOPULL,
		.Speed	= GPIO_SPEED_HIGH
	});

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
	
	// Turn SPI1 Clock on
	__HAL_RCC_SPI1_CLK_ENABLE();
	
	// Initialize SPI1
	if (HAL_SPI_Init(&hspi1) != HAL_OK)
	{
		printf( "SPI Failed\n" );
		while(1);
	}
}

// Initialize I2C1 for use with lora-bone
// I2C Speed will be 400 kbit/s
void bone_initI2c1()
{
	static bool initialized = false;

	// Only do initialization once
	if (initialized)
		return;
	initialized = true;

	// Turn GPIOB Clock on
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	// Configure PB6 for SCL
	HAL_GPIO_Init(GPIOB, &(GPIO_InitTypeDef){
			.Mode = GPIO_MODE_AF_OD,
			.Pin = GPIO_PIN_6,
			.Pull = GPIO_NOPULL,
			.Speed = GPIO_SPEED_HIGH
		});
	// Configure PB7 for SDA
	HAL_GPIO_Init(GPIOB, &(GPIO_InitTypeDef){
			.Mode = GPIO_MODE_AF_OD,
			.Pin = GPIO_PIN_7,
			.Pull = GPIO_NOPULL,
			.Speed = GPIO_SPEED_HIGH
		});

	hi2c1.Instance = I2C1;
	hi2c1.Init = (I2C_InitTypeDef){
		.ClockSpeed = 400000,
		.DutyCycle = I2C_DUTYCYCLE_2,
		.AddressingMode = I2C_ADDRESSINGMODE_7BIT,
		.GeneralCallMode = I2C_GENERALCALL_DISABLE,
		.DualAddressMode = I2C_DUALADDRESS_DISABLE
	};

	// Turn I2C1 clock on
	__HAL_RCC_I2C1_CLK_ENABLE();
	// Init I2C1
	if (HAL_I2C_Init(&hi2c1) != HAL_OK)
	{
		printf("I2C Failed\n");
		while(1);
	}
}	

// Initialize TIM2 for use with ibm-lmic
void bone_initTim2()
{
	static bool initialized = false;

	// Only do initialization once
	if (initialized)
		return;
	initialized = true;

	// Turn Timer 2 clock on
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

	// Initialize Timer 2
	HAL_TIM_Base_Init(&htim2);

	// Enable Timer 2 Interrupt
	HAL_NVIC_EnableIRQ(TIM2_IRQn);

	// Start Timer with Interrupt
	HAL_TIM_Base_Start_IT(&htim2);

}

// Initialize lora-bone bsp
// This initializes GPIOC Pin 13 for LED,
// setups the clock for our STM32F1 Board
void bone_init()
{
	// Only do initialization once
	if (bone_bsp_initialized)
		return;
	bone_bsp_initialized = true;

	// Turn GPIOC Clock on
	__HAL_RCC_GPIOC_CLK_ENABLE();
	// Initialize GPIOC Pin 12 as Output Push-Pull
	// The LED is connected to this Pin
	HAL_GPIO_Init(GPIOC, &(GPIO_InitTypeDef){
		.Pin	= GPIO_PIN_13,
		.Mode	= GPIO_MODE_OUTPUT_PP,
		.Pull	= GPIO_NOPULL,
		.Speed	= GPIO_SPEED_HIGH
	});

}

void bone_drive_CS1(bool value)
{
	if (!bone_bsp_initialized)
		return;

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, (value)?GPIO_PIN_SET:GPIO_PIN_RESET);
}

void bone_set_led(bool value)
{
	if (!bone_bsp_initialized)
		return;

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, (value)?GPIO_PIN_RESET:GPIO_PIN_SET);
}

void bone_toggle_led()
{
	if (!bone_bsp_initialized)
		return;

	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
}

