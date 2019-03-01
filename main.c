/* Includes ------------------------------------------------------------------*/
#include "encoder_module.h"
#include "pwm_module.h"
#include "bridge_module.h"
#include "speed_sample_process.h"
#include "motor_control_process.h"
#include "uart_module.h"
#include "stm32f4_discovery.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define CONTROL 1
#define SPEED 0

#define SAMPLING_FREQ 100

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void init_systick(void);
void testAction(void);
void delay(uint32_t counts);

int main(void)
{
	uint16_t prescaler = 1000;
	uint16_t period = 1000;

	configure_pwm_af();
	configure_pwm_freq(prescaler, period);

	configure_pwm_ccr(1, 0);
	configure_pwm_ccr(2, 0);
	configure_pwm_ccr(3, 0);
	configure_pwm_ccr(4, 0);

	start_pwm();

	init_encoders();
	init_bridge_pins();
	init_systick();
	init_uart(9600);

	if (SPEED) {
		do_speed_sampling(SAMPLING_FREQ);
	} else if (CONTROL) {
		do_motor_control(SAMPLING_FREQ);
	}

	while(1); // oups
}

void init_systick(void) {
	if (SysTick_Config(SystemCoreClock / SAMPLING_FREQ))
	{
		/* Capture error */
		while (1);
	}
}

void SysTick_Handler(void) {
	if (CONTROL) {
		controlMotors();
	} else if (SPEED) {
		mesureSpeeds();
	} else {
		testAction();
	}
}

int state = 1;
void testAction(void) {
	if (state) {
		GPIO_SetBits(GPIOD, GPIO_Pin_1);
		state = 0;
	} else {
		GPIO_ResetBits(GPIOD, GPIO_Pin_1);
		state = 1;
	}
}

void delay(uint32_t counts) {
	for (int i = 0; i < counts; i++);
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
