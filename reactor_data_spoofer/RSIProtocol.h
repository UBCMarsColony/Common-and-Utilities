#ifndef RSI_PROTOCOL_H
#define RSI_PROTOCOL_H

#include <stdint.h>

typedef struct RSIPacket
{
	uint8_t p_class;
	uint8_t p_descriptor;
	uint8_t n_data;
	uint8_t l_data;
	uint8_t data[40];
};

typedef enum RSIPacketClass
{
	STATUS = 'S',
	DATA   = 'D'
};

typedef enum RSIStatusPacketDescriptor
{
	INITIALIZAED   = 1,
	STARTING       = 2,
	STD_OPERATION  = 3,
	STOPPING       = 4,
	EMERGENCY_STOP = 5
};

typedef enum RSIDataPacketDescriptor
{
	TEMPERATURE = 1,
	FLOW_RATE   = 2,
	PRESSURE    = 3,
	SCALE_MASS  = 4
};

/*
 * Specific Data Packet Formats
 */

typedef struct TemperatureFrame
{
	uint16_t temperature_id;
	uint32_t timestamp;
	float    temperature;
};

typedef struct FlowRateFrame
{
	uint16_t flowrate_id;
	uint32_t timestamp;
	float flow_rate;
};

typedef struct PressureFrame
{
	uint16_t pressure_id;
	uint32_t timestamp;
	float pressure;
};

typedef struct ScaleFrame
{
	uint16_t scale_id;
	uint32_t timestamp;
	float mass;
};

#endif // RSI_PROTOCOL_H
