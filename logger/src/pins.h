#ifndef PINS_H
#define PINS_H

/* Connect the SD card reader like follows:
 	MOSI - pin 11
 	MISO - pin 12
 	PIN_CLOCK - pin 13
 	CS - pin 4
*/

/* Connect the start button and the buzzer like follows:
 	Activation button - pin 10
 	LED - pin 5
 	Buzzer - pin 6
*/


#define RECORD_SECONDS 30
#define AFT_SIGNAL_DELAY_SECS 7

#define PIN_LED 5
#define PIN_BUZZ 7
#define PIN_MOSFET 6
#define PIN_CLOCK	2
#define PIN_DATAOUT	3
#define PIN_BUZZ 7

// Serial pins for radio
#define PIN_TX 9
#define PIN_RX 8

// Part of the SD pins
#define PIN_SDCS 10

#endif
