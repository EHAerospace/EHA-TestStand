#include <SoftwareSerial.h>
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

#define PIN_LED 5
#define PIN_BUZZ 7
#define PIN_MOSFET 6
#define PIN_CLOCK	2
#define PIN_DATAOUT	3

#define LED 5
#define TONE 7

// Part of the SD pins
#define PIN_SDCS 10

const char* payload = "PAUPER AD ASTRA"; // 15 bytes
char buffer[16];

SoftwareSerial xrf(8,9);

File file;

HX711 scale(PIN_DATAOUT, PIN_CLOCK);
int zero_factor;

void blink()
{
    tone(TONE, 1000, 50);
    digitalWrite(LED, HIGH);
    delay(200);
    noTone(TONE);
    digitalWrite(LED, LOW);
    delay(200);
}

void err() {
    tone(TONE, 1500, 50);
    digitalWrite(LED, HIGH);
    delay(50);
    noTone(TONE);
    digitalWrite(LED, LOW);
    delay(50);
}

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
	pinMode(PIN_LED, OUTPUT);
	pinMode(PIN_BUZZ, OUTPUT);
	pinMode(PIN_MOSFET, OUTPUT);

	digitalWrite(PIN_MOSFET, LOW);

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

void logger() 
{ 
	// Wait until the activation signal is on, and take the specified samples
	Serial.println("Starting sampling routine");
	take_samples(scale, file, 80*RECORD_SECONDS);

	// Closes the file:
	file.print("Final timestamp: ");
	file.println(millis());
	file.close();
	Serial.print("Program finalized");
}

void setup() {

    // Debug
    Serial.begin(9600);

    // Serial for xrf radio
    xrf.begin(9600);

    Serial.println("EH Launcher ON");

    // GPIO
    pinMode(LED, OUTPUT);
    blink();

    pinMode(PIN_MOSFET, OUTPUT);
    pinMode(TONE, OUTPUT);

    setup_peripherals();

    // Prepare the scale and log the calculated scale factor to the serial
    Serial.println("Calibrating sensor...");
    scale.set_scale();
    scale.tare();			//Reset the scale to
    zero_factor = scale.read_average(); //Get a baseline reading

    make_signals(10, 500);
    Serial.println();
    Serial.print("Creating log file...");
    // Create the file and add the header
    String filename = get_filename();
    file = SD.open(filename.c_str(), FILE_WRITE);
    file.print("Tare: ");
    file.println(zero_factor);
    file.print("Starting timestamp: ");
    file.println(millis());
    file.println("From lecture position:");
    Serial.println(" Done.");

    Serial.print("ReferenceValue = "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
    Serial.println(zero_factor);
}

void loop() {

    do {
        // Serial.println(xrf.read());
        buffer[0] = xrf.read();
    }
    while (buffer[0] != 'P');
    
    // ok we have data, check it
    // wait for answer with 1 sec timeout
    bool ok = true;
    bool finish = false;
    int i = 1;

    while(!finish && ok)
    {
        char val = xrf.read();
        
        if (val == -1) {
            Serial.println("NULl");
            continue;
        }         

        Serial.print("value: ");
        Serial.print(val);
        Serial.print(" str: ");
        Serial.println(payload[i]);

        finish = i == 14;

        if(ok && val == payload[i]) 
            i++;
        else
            ok = false;
        
    }

    // if (strncmp(buffer+8, payload, 15) == 0)
    if (ok)
    {
        // send confirmation
        delay(100);
        xrf.print('K');

        Serial.println("Launch ORDER");
        // listen
        Serial.println("Launch OK LSTN GO");
        // wait for answer with 1 sec timeout
        long now = millis();
        do {
            buffer[0] = xrf.read();
            if (millis() - now > 3000) {
                while (1)
                {
                    err();
                }
            }
        } while (buffer[0] == -1);

        // ok we have data, check it
        if (buffer[0] == 'G')
        {

            // OK LAUNCHING & start logger
            Serial.println("Launch GO");
            digitalWrite(PIN_MOSFET, HIGH); // Start ignition
            logger();

            while (1);
        }
        else
        {
            Serial.println("Error: Bad sync!");
            while (1)
            {
                err();
            }
        }
    }
    else // bad data, error
    {
        Serial.println("Error: Bad password!");
        while (1)
        {
            err();
        }
    }
}

