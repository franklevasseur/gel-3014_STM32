#include "speed_sample_process.h"

#define SAMPLE_SZ 100
#define TRANS_UART 1

int16_t measuredSpeeds[4][SAMPLE_SZ] = {0};
uint32_t samples_count = 0;
int samplingHasStarted = 0;

uint32_t sampling_frequency;

void transmitSamples(void);

void do_speed_sampling(uint32_t p_sampling_frequency)
{
	sampling_frequency = p_sampling_frequency;

	configure_pwm_ccr(1, 500);
	configure_pwm_ccr(2, 500);
	configure_pwm_ccr(3, 500);
	configure_pwm_ccr(4, 500);

	set_motor_action(1, CW);
	set_motor_action(2, CW);
	set_motor_action(3, CW);
	set_motor_action(4, CW);

	while(1) {
		if (samples_count >= SAMPLE_SZ) {

			if (TRANS_UART) {
				transmitSamples();
			}

			while(1); // capture sampling done
		}
	}
}

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
		for (uint8_t i = 0; i < 4; i++) {
			int16_t ticksPerSecond = counts[i] * sampling_frequency;
			measuredSpeeds[i][samples_count] = ticksPerSecond;
		}
		samples_count++;
	}

	resetCounts();
}

void transmitSamples() {
	// transmit all samples by UART
	for (int m = 0; m < 4; m++) {
		for (int s = 0; s < SAMPLE_SZ; s++) {

			int16_t sample = measuredSpeeds[m][s];
			char firstByte = (char)(sample & 0xFF);
			char secondByte = (char)((sample >> 8) & 0xFF);

			uart_write(firstByte);
			uart_write(secondByte);
		}
	}
}
