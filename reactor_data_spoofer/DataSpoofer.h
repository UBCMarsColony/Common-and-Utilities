#ifndef DATA_SPOOFER_H
#define DATA_SPOOFER_H

#include <stdint.h>

void spoof_init();

/*
 * Description: Spoofs temperature data.
 * Parameters: N - The number of datapoints to spoof
 *             buffer - A pointer to an array
 */

void spoof_temperature(uint8_t *buffer, uint8_t length);
void spoof_flowrate(uint8_t *buffer, uint8_t length);
void spoof_scale(uint8_t *buffer, uint8_t length);

#endif //DATA_SPOOFER_H

