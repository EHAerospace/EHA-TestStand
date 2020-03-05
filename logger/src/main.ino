// External libraries
#include <SoftwareSerial.h>
#include <HX711.h>
#include <SPI.h>
#include <SD.h>

// Project libraries
#include <settings.h>
#include <pins.h>
#include <error.h>

HX711 scale(PIN_DATAOUT, PIN_CLOCK);
SoftwareSerial xrf(PIN_RX, PIN_TX);
File logger_file;

#include <init.h>
#include <radio.h>
#include <logger.h>

void setup()
{
	// Start peripherals setup
	int res = setup_peripherals();
	if (res != NO_ERROR)
	{
		// There has been some error
		led_and_buzz_error(res, true); //blink and buzz "res" times
	}

	led_and_buzz_normal(false, false); // Stop LED and buzz for normal signaling
	delay(1000);

	// Start radio standby for radio comm. At this point the igniter can be armed.
	led_and_buzz_normal(true, false); // Start only LED for normal signaling
	res = radio();
	while (res != NO_ERROR)
	{
		led_and_buzz_error(res, true);	//blink and buzz "res" times
	}

	logger();

	led_and_buzz_error(BLINK_ALL_WORK_DONE, true); //blink and buzz 100 times
}

void loop()
{
}
