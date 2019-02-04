#include "speed_sample_process.h"

#define SAMPLE_SZ 1000

int32_t measuredSpeeds[4][SAMPLE_SZ] = {0};
uint32_t samples_count = 0;
int samplingHasStarted = 0;

void do_speed_sampling(void)
{
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

	if (samplingHasStarted) {
		for (uint8_t i = 0; i < 4; i++) {
			measuredSpeeds[i][samples_count] = counts[i];
		}
	}

	resetCounts();
}
