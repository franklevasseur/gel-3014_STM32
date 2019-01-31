/* Includes ------------------------------------------------------------------*/
#include "stm32f4_discovery.h"
#include "pwm_module.h"
#include "motor_module.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//speeds
volatile int16_t leftCount;
volatile int16_t rightCount;
volatile int16_t fwdCount;
volatile int16_t rotCount;
//distances
volatile int32_t leftTotal;
volatile int32_t rightTotal;
volatile int32_t fwdTotal;
volatile int32_t rotTotal;

// local variables
static volatile int16_t oldLeftEncoder;
static volatile int16_t oldRightEncoder;
static volatile int16_t leftEncoder;
static volatile int16_t rightEncoder;
static volatile int16_t encoderSum;
static volatile int16_t encoderDiff;

/* Private function prototypes -----------------------------------------------*/
void encodersInit(void);
void encodersReset(void);
/* Private functions ---------------------------------------------------------*/

int main(void)
{
	uint16_t CCR_Val = 333;
	uint16_t prescaler = 1249;
	uint16_t period = 666;
	init_pwm(prescaler, period, CCR_Val);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	/* Configure PD12, PD13, PD14 and PD15 in output pushpull mode */
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	if (SysTick_Config(SystemCoreClock / 100)) // fs = 100 Hz
	{
		/* Capture error */
		while (1);
	}

	while (1)
	{}
}

int state = 1;
void SysTick_Handler(void) {
	if (state) {
		GPIO_SetBits(GPIOD, GPIO_Pin_15);
		state = 0;
	} else {
		GPIO_ResetBits(GPIOD, GPIO_Pin_15);
		state = 1;
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

  while (1)
  {}
}
#endif
