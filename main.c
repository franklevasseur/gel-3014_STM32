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

uint16_t ccrs[4] = {0};

int main(void)
{
	uint16_t prescaler = 1249;
	uint16_t period = 1000;

	configure_timer_af();
	configure_timer_freq(prescaler, period);

	configure_timer_pwm(1, ccrs[0]);
	configure_timer_pwm(2, ccrs[1]);
	configure_timer_pwm(3, ccrs[2]);
	configure_timer_pwm(4, ccrs[3]);

	start_pwm();

	init_encoders();

	if (SysTick_Config(SystemCoreClock / 100))
	{
		/* Capture error */
		while (1);
	}

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_SetBits(GPIOD, GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4);
	GPIO_ResetBits(GPIOD, GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9);

	while (1)
	{
	}
}

uint32_t consignes[4] = {50, 50, 50, 50};

uint32_t kp = 1;
uint32_t ki = 1;
int32_t cumulativesErr[4] = {0};

void SysTick_Handler(void) {

	int32_t counts[4] = {0};
	getCounts(counts);

	for (uint8_t i = 0; i < 4; i++) {
		int32_t err = consignes[i] - counts[i];
		cumulativesErr[i] += err;
		int32_t correction = (err * kp) + (cumulativesErr[i] * ki);

		ccrs[i] += correction;

		if (ccrs[i] > 1000) {
			ccrs[i] = 1000;
		} else if (ccrs[i] < 0) {
			ccrs[i] = 0;
		}

		configure_timer_pwm(i, ccrs[i]);
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
