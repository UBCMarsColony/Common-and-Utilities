#include <stdlib.h>
#include <stdint.h>

#include "./RSIProtocol.h"
#include "./ReactorLocaleIDs.h"
#include "./DataSpoofer.h" 
// #include "./CommandParser.h"

void setup() {
	Serial.begin(2000000);
	spoof_init();
}

void loop() 
{	
	if (Serial.available());	
		//parse_command();

	// SENDING DATA
	// We want to routinely update
	// the UI with data; that happens here.
	RSIPacket packet = RSIPacket();
	packet.p_class = DATA;
	
	// Temperature
	packet.p_descriptor = TEMPERATURE;
	packet.n_data = NUM_TEMP_LOCALES;
	packet.l_data = sizeof(TemperatureFrame);
	spoof_temperature((uint8_t *) packet.data, 
			  packet.n_data * packet.l_data);

  	Serial.print("RSIP>>");
  	Serial.write((uint8_t *) &packet, sizeof(RSIPacket));
 
	delay(rand() % 200 + 50);

	// Flow Rate
	packet.p_descriptor = FLOW_RATE;
	packet.n_data = NUM_FLOW_LOCALES;
	packet.l_data = sizeof(FlowRateFrame);
	spoof_flowrate((uint8_t *) packet.data,
		       packet.n_data * packet.l_data);

	Serial.print("RSIP>>");
  	Serial.write((uint8_t *) &packet, sizeof(RSIPacket));
 
	delay(rand() % 200 + 50);
	
	// Scale
	packet.p_descriptor = SCALE_MASS;
	packet.n_data = NUM_SCALE_LOCALES;
	packet.l_data = sizeof(ScaleFrame);
	spoof_scale((uint8_t *) packet.data,
		       packet.n_data * packet.l_data);

  	Serial.print("RSIP>>");
  	Serial.write((uint8_t *) &packet, sizeof(RSIPacket));
  	
	delay(rand() % 200 + 50);
}
