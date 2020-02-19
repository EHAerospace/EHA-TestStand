#include <SoftwareSerial.h>
#include <HX711.h>
#include <SPI.h>
#include <SD.h>

#include <pins.h>
#include <init.h>
#include <error_init.h>
#include <radio.h>
#include <error_radio.h>
#include <logger.h>
#include <settings.h>

#if DEBUG_ACTIVE
	#define DEBUG_LOG(x) 		Serial.print(x)
	#define DEBUG_LOG_LINE(x) 	Serial.println(x)
#else
	#define DEBUG_LOG(x)
	#define DEBUG_LOG_LINE(x)
#endif

const char* payload = "PAUPER AD ASTRA"; // 15 bytes
char buffer[16];

SoftwareSerial xrf(8,9);

File file;

HX711 scale(PIN_DATAOUT, PIN_CLOCK);
int zero_factor;

void blink()
{
    tone(PIN_BUZZ, 1000, 50);
    digitalWrite(PIN_LED, HIGH);
    delay(200);
    noTone(PIN_BUZZ);
    digitalWrite(PIN_LED, LOW);
    delay(200);
}

void signal_ok() 
{
    make_signals(2, 200);
}  

void signal_error() 
{
    make_signals(50, 100);
}

String get_filename()
{
	unsigned int i = 0;
	while(SD.exists(String("data") + String(i) + String(".txt")))
	{
		i++;
	}
        DEBUG_LOG_LINE(String("filename: data") + String(i));
	return String("data") + String(i) + String(".txt");
}


void make_signals(int signal_count, int interval)
{
	for (int f = 0; f < signal_count; f++)
	{
		tone(PIN_BUZZ, BUZZ_FREQUENCY, interval/2);
		delay(interval/2);
		noTone(PIN_BUZZ);
		delay(interval/2);
	}
}



void setup() {
	int res = init();
	if(res != 0) {
		// TODO: error check
	}

	res = radio();
	while(res != 0) {
		// TODO: pitiditos 'res' veces
	
		res = radio();
	}

	logger();

	// TODO: ALL OK
}

void loop() {

    }

