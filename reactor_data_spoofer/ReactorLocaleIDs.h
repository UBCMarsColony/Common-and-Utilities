#ifndef REACTOR_LOCALE_IDS_H
#define REACTOR_LOCALE_IDS_H


typedef enum TemperatureLocaleID
{
	MIXER   = 1,
	INFLOW  = 2,
	REACTOR = 3,
	OUTFLOW = 4,

	NUM_TEMP_LOCALES = 4
};

typedef enum FlowRateLocaleID
{
	CO2     = 1,
	ARGON   = 2,
	H20     = 3,
	METHANE = 4,

	NUM_FLOW_LOCALES = 4
};


typedef enum PressureLocaleID
{
	SOMEWHERE = 1,

	NUM_PRES_LOCALES = 1
};

typedef enum ScaleLocaleID
{
	WATER = 1,

	NUM_SCALE_LOCALES = 1
};

#endif
