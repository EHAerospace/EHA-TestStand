#include <HX711.h>
#include <SPI.h>
#include <SD.h>

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
#define BUZZ_FREQUENCY 440

#define PIN_ACTIVATION_SIGNAL 4
#define PIN_LED 5
#define PIN_BUZZ 7
#define PIN_POWER 8
#define PIN_CLOCK	2
#define PIN_DATAOUT	3

// Part of the SD pins
#define PIN_SDCS 10

String get_filename()
{
	unsigned int i = 0;
	while(SD.exists(String("data") + String(i) + String(".txt")))
	{
		i++;
	}
        Serial.println(String("filename: data") + String(i));
	return String("data") + String(i) + String(".txt");
}

void setup_peripherals()
{
	// Setup the Serial
	Serial.begin(9600);

        Serial.println("EH Launcher ON");

	while (!Serial);
	Serial.println("Serial initialized.");

	// Setup SD card
	Serial.print("Initializing SD card...");
        while (!SD.begin(PIN_SDCS))
	{
		Serial.println("SD Failed!");
                make_signals(5, 50);
	}
	Serial.println("Done.");

	// Setup pins
	Serial.print("Setting pin modes...");
	pinMode(PIN_ACTIVATION_SIGNAL, INPUT);
	pinMode(PIN_LED, OUTPUT);
	pinMode(PIN_BUZZ, OUTPUT);
	pinMode(PIN_POWER, OUTPUT);
	digitalWrite(PIN_POWER, HIGH);
	Serial.println(" Done.");
}

void make_signals(int signal_count, int interval)
{
	for (int f = 0; f < signal_count; f++)
	{
		digitalWrite(PIN_LED, HIGH);
		tone(PIN_BUZZ, BUZZ_FREQUENCY, interval/2);
		delay(interval/2);
		digitalWrite(PIN_LED, LOW);
		noTone(PIN_BUZZ);
		delay(interval/2);
	}
}

void take_samples(HX711& scale, File& file, int sample_count)
{
	for (int f = 0; f < sample_count; f++)
	{
		file.print(scale.read());
		file.print(":");
	}
}



void setup() 
{
        setup_peripherals();

	// Initializes HX711
	HX711 scale(PIN_DATAOUT, PIN_CLOCK);
  
	// Prepare the scale and log the calculated scale factor to the serial
	Serial.println("Calibrating sensor...");
	scale.set_scale();
	scale.tare();			//Reset the scale to
	long zeroFactor = scale.read_average(); //Get a baseline reading
	
	Serial.println(" Done.");
	Serial.print("ReferenceValue = "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
	Serial.println(zeroFactor);

	make_signals(10, 500);

	Serial.println();
	Serial.print("Creating log file...");

	// Create the file and add the header
	String filename = get_filename();
	File file = SD.open(filename.c_str(), FILE_WRITE);
	file.print("Tare: ");
	file.println(zeroFactor);
	file.print("Starting timestamp: ");
	file.println(millis());
	file.println("From lecture position:");
	Serial.println(" Done.");

	// Wait until the activation signal is on, and take the specified samples
	Serial.println("Waiting until the start signal is on...");
        make_signals(10, 250);
        
	while(digitalRead(PIN_ACTIVATION_SIGNAL) == LOW);
        Serial.println("Signal received, waiting delay...");
        make_signals(10, 100);

        delay(AFT_SIGNAL_DELAY_SECS*1000);

	Serial.println("Starting sampling routine");
	take_samples(scale, file, 80*RECORD_SECONDS);

	// Closes the file:
	file.print("Final timestamp: ");
	file.println(millis());
	file.close();
	Serial.print("Program finalized");
}

void loop()
{
}