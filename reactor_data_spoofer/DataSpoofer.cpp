#include "./DataSpoofer.h"

#include <stdint.h>  // uint##_t
#include <stdlib.h>  // srand, rand
#include <Arduino.h>
#include <time.h>

#include "./ReactorLocaleIDs.h"  // NUM_XXXX_LOCALES
#include "./RSIProtocol.h"

const float
	MAX_TEMP = 400,  // Celsius
	MAX_PRES = 200,  // kiloPascals
	MAX_MASS = 100,  // g
	MAX_FLOW = 2.0;    // L/s

float temperature[NUM_TEMP_LOCALES],
      flowrate[NUM_FLOW_LOCALES],
      pressure[NUM_PRES_LOCALES],
      scale[NUM_SCALE_LOCALES];


void spoof_init()
{
	// Initialize our RNG
	srand(time(NULL));
}

void spoof_temperature(uint8_t *buffer, uint8_t length)
{
	// Elapsed time calculations
	static unsigned long last = 0;
	unsigned long now = millis();
	float dt = (now - last) * 1E-3;
	last = now;

	// Temperature Spoofing
	TemperatureFrame *frames = (TemperatureFrame *) buffer;  // Iterate over buffer by pointer
	for (int i = 0; i < NUM_TEMP_LOCALES; i++)
	{
		// BEHAVIOUR: Plateau to some temperature
		float dTdt = (MAX_TEMP*((i * 1.0 / NUM_TEMP_LOCALES)) - temperature[i])  //...
				* 1.0E-2;  // Modify this to change response speed
		temperature[i] += dTdt * dt;

		// Populate the array
		frames[i].temperature_id = i + 1;
		frames[i].timestamp      = millis();
		float noise = rand() % 100 / 100.0 - 0.5;  // Up to half a Ceslius
		frames[i].temperature    = temperature[i] + noise;
	}
}

void spoof_flowrate(uint8_t *buffer, uint8_t length)
{
	// Elapsed time calculations
	static unsigned long last = 0;
	unsigned long now = millis();
	float dt = (now - last) * 1E-3;
	last = now;

	// Flow Rate Spoofing
	FlowRateFrame *frames = (FlowRateFrame *) buffer;  // Iterate over buffer by pointer
	for (int i = 0; i < NUM_FLOW_LOCALES; i++)
	{
		// BEHAVIOUR: Approach and oscillate about max flow rate
		float d2mdt2 = (MAX_FLOW - flowrate[i])  //...
				* 0.5;  // Modify this to change response speed
		flowrate[i] += d2mdt2 * dt;

		// Populate the array
		frames[i].flowrate_id = i + 1;
		frames[i].timestamp   = millis();
		float noise = rand() % 100 / 500.0 - .1;  // +- 0.1 L/s
		frames[i].flow_rate   = flowrate[i] + noise;
	}
}

void spoof_scale(uint8_t *buffer, uint8_t length)
{
	// Elapsed time calculations
	static unsigned long last = 0;
	unsigned long now = millis();
	float dt = (now - last) * 1E-3;
	last = now;

	// Scale Mass Spoofing
	ScaleFrame *frames = (ScaleFrame *) buffer;  // Iterate over buffer by pointer
	for (int i = 0; i < NUM_SCALE_LOCALES; i++)
	{
		// BEHAVIOUR: Linearly increase until max value is reached
		float dmdt = (scale[i] < MAX_MASS)  //...
				* 0.1;  // Modify this to change response speed
		scale[i] += dmdt * dt;

		// Populate the array
		frames[i].scale_id  = i + 1;
		frames[i].timestamp = millis();
		float noise = rand() % 100 / 5000.0 - 0.01;  // Up to 0.01 grams
		frames[i].mass      = scale[i] + noise;
	}
}

