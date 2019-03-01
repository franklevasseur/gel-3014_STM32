#include "speed_sample_process.h"

#define SAMPLE_SZ 100
#define TRANS_UART 1

int16_t measuredSpeeds[4][SAMPLE_SZ] = {0};
uint32_t samples_count = 0;
int samplingHasStarted = 0;

uint32_t sampling_frequency;

int32_t ccrs[4] = {1000, 1000, 1000, 1000};

void transmitSamples(void);

void do_speed_sampling(uint32_t p_sampling_frequency)
{
	// On prend la led bleue (PD15) pour debugger
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_SetBits(GPIOD, GPIO_Pin_15);

	sampling_frequency = p_sampling_frequency;

	configure_pwm_ccr(1, ccrs[0]);
	configure_pwm_ccr(3, ccrs[1]);
	configure_pwm_ccr(4, ccrs[2]);
	configure_pwm_ccr(2, ccrs[3]);

	set_motor_action(1, CCW);
	set_motor_action(2, CCW);
	set_motor_action(3, CCW);
	set_motor_action(4, CCW);

//	for (int i = 0; i < 0xFFFFFF; i++); // délai pour s'assurer qu'on est à pleine vitesse
//	samplingHasStarted = 1;

	while(1) {
		if (samples_count >= SAMPLE_SZ) {

			GPIO_ResetBits(GPIOD, GPIO_Pin_15);

			if (TRANS_UART) {
				transmitSamples();
			}

			set_motor_action(1, BLOCK);
			set_motor_action(2, BLOCK);
			set_motor_action(3, BLOCK);
			set_motor_action(4, BLOCK);
			while(1); // capture sampling done
		}
	}
}

int compteur_de_sample = 0;

void mesureSpeeds(void)
{
	int32_t counts[4] = {0};
	getCounts(counts);

	samplingHasStarted = samplingHasStarted
			|| !(counts[0] == 0
					&& counts[1] == 0
					&& counts[2] == 0
					&& counts[3] == 0);

	if (samplingHasStarted && samples_count < SAMPLE_SZ) {

		int ajuste_duty = compteur_de_sample >= 10;

		for (uint8_t motor = 0; motor < 4; motor++) {
			int16_t ticksPerSecond = counts[motor] * sampling_frequency;
			measuredSpeeds[motor][samples_count] = ticksPerSecond;

//			if (ajuste_duty) {
//				ccrs[motor] = ccrs[motor] - 5 < 0 ? 0 : ccrs[motor] - 5;
//				compteur_de_sample = 0;
//			}
		}

		ajuste_duty = 0;
		compteur_de_sample++;
		samples_count++;
	}

	configure_pwm_ccr(1, ccrs[0]);
	configure_pwm_ccr(3, ccrs[1]);
	configure_pwm_ccr(4, ccrs[2]);
	configure_pwm_ccr(2, ccrs[3]);

	resetCounts();
}

void transmitSamples() {
	// transmit all samples by UART
	for (int m = 0; m < 4; m++) {
		for (int s = 0; s < SAMPLE_SZ; s++) {

			int16_t sample = measuredSpeeds[m][s];
			transmit_sample(sample);
		}
	}
}
