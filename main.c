/* Includes ------------------------------------------------------------------*/
#include "stm32f4_discovery.h"
#include "pwm_module.h"
#include "motor_module.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void delay(uint32_t counts);

int main(void)
{
	uint16_t CCR_Val = 333;
	uint16_t prescaler = 1249;
	uint16_t period = 666;
	init_pwm(prescaler, period, CCR_Val);

	init_encoders();

	if (SysTick_Config(SystemCoreClock / 100)) // fs = 30 Hz
	{
		/* Capture error */
		while (1);
	}

	while (1)
	{}
}

int state = 1;
void SysTick_Handler(void) {

	delay(0xFFFFF);

	uint32_t counts[4] = {0};
	uint32_t zeroReference = getCounts(counts);

	uint32_t clockwiseCount = 0;
	uint32_t counterClockwiseCount = 0;
	int motorIndex = 2;
	if (counts[motorIndex] > zeroReference) {
		clockwiseCount = counts[motorIndex] - zeroReference;
	} else {
		counterClockwiseCount = zeroReference - counts[motorIndex];
	}

	resetCounts();
}

void delay(uint32_t counts) {
	for (int i = 0; i < 0xFFFFF; i++);
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
