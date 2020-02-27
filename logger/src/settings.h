#ifndef SETTINGS_H
#define SETTINGS_H

#define DEBUG_ACTIVE 1
#define SERIAL_BAUD_RATE 115200
#define RADIO_BAUD_RATE 9600

#if DEBUG_ACTIVE
	#define DEBUG_LOG(x) 		Serial.print(x)
	#define DEBUG_LOG_LINE(x) 	Serial.println(x)
#else
	#define DEBUG_LOG(x)
	#define DEBUG_LOG_LINE(x)
#endif

#endif
