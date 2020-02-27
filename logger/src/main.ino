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
File file;
const char *payload = "PAUPER AD ASTRA"; // 15 bytes
char buffer[16];
int zero_factor;

#include <init.h>
#include <radio.h>
#include <logger.h>

void setup()
{
	// Start peripherals setup
	int res = setup_peripherals();
	if (res != 0)
	{
		// There has been some error
		blink_led_and_buzz_error(res, true); //blink and buzz "res" times
	}

	blink_led_and_buzz_normal(false, false); // Stop blink and buzz for normal signaling
	delay(1000);

	// Start radio standby for radio comm. At this point the igniter can be armed.
	blink_led_and_buzz_normal(true, false); // Start only blink for normal signaling
	res = radio();
	while (res != 0)
	{
		blink_led_and_buzz_error(res, true);	//blink and buzz "res" times
		blink_led_and_buzz_normal(true, false); // Start only blink for normal signaling
		res = radio();
	}

	logger();

	blink_led_and_buzz_error(ALL_WORK_DONE, true); //blink and buzz 100 times
}

void loop()
{
}
