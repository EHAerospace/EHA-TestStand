#include <SoftwareSerial.h>
#include <HX711.h>
#include <SPI.h>
#include <SD.h>

#include <settings.h>

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

#if DEBUG_ACTIVE
	#define DEBUG_LOG(x) 		Serial.print(x)
	#define DEBUG_LOG_LINE(x) 	Serial.println(x)
#else
	#define DEBUG_LOG(x)
	#define DEBUG_LOG_LINE(x)
#endif

#define RECORD_SECONDS 30
#define AFT_SIGNAL_DELAY_SECS 7
#define BUZZ_FREQUENCY 440

#define PIN_LED 5
#define PIN_BUZZ 7
#define PIN_MOSFET 6
#define PIN_CLOCK	2
#define PIN_DATAOUT	3

#define PIN_BUZZ 7

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

void setup_peripherals()
{
	// Setup the Serial
	Serial.begin(9600);

        DEBUG_LOG_LINE("EH Launcher ON");

	while (!Serial);
	DEBUG_LOG_LINE("Serial initialized.");

	// Setup SD card
	DEBUG_LOG("Initializing SD card...");
        while (!SD.begin(PIN_SDCS))
	{
		DEBUG_LOG_LINE("SD Failed!");
                make_signals(5, 50);
	}
	DEBUG_LOG_LINE("Done.");

	// Setup pins
	DEBUG_LOG("Setting pin modes...");
	pinMode(PIN_LED, OUTPUT);
	pinMode(PIN_BUZZ, OUTPUT);
	pinMode(PIN_MOSFET, OUTPUT);

	digitalWrite(PIN_MOSFET, LOW);

	DEBUG_LOG_LINE(" Done.");
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
	DEBUG_LOG_LINE("Starting sampling routine");
	take_samples(scale, file, 80*RECORD_SECONDS);

	// Closes the file:
	file.print("Final timestamp: ");
	file.println(millis());
	file.close();
	DEBUG_LOG("Program finalized"); 
	signal_ok();
}

void setup() {

    // Debug
    Serial.begin(9600);

    // Serial for xrf radio
    xrf.begin(9600);

    DEBUG_LOG_LINE("EH Launcher ON");

    // GPIO
    pinMode(PIN_LED, OUTPUT);
    blink();

    pinMode(PIN_MOSFET, OUTPUT);
    pinMode(PIN_BUZZ, OUTPUT);

    setup_peripherals();

    // Prepare the scale and log the calculated scale factor to the serial
    DEBUG_LOG_LINE("Calibrating sensor...");
    scale.set_scale();
    scale.tare();			//Reset the scale to
    zero_factor = scale.read_average(); //Get a baseline reading

    make_signals(10, 500);
    DEBUG_LOG_LINE();
    DEBUG_LOG("Creating log file...");
    // Create the file and add the header
    String filename = get_filename();
    file = SD.open(filename.c_str(), FILE_WRITE);
    file.print("Tare: ");
    file.println(zero_factor);
    file.print("Starting timestamp: ");
    file.println(millis());
    file.println("From lecture position:");
    DEBUG_LOG_LINE(" Done.");

    DEBUG_LOG("ReferenceValue = "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
    DEBUG_LOG_LINE(zero_factor);
}

void loop() {

    do {
        // DEBUG_LOG_LINE(xrf.read());
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
            DEBUG_LOG_LINE("NULL_VALUE");
            continue;
        }         

        DEBUG_LOG("value: ");
        DEBUG_LOG(val);
        DEBUG_LOG(" str: ");
        DEBUG_LOG_LINE(payload[i]);

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

        DEBUG_LOG_LINE("Launch ORDER");
        // listen
        DEBUG_LOG_LINE("Launch OK LSTN GO");
        // wait for answer with 1 sec timeout
        long now = millis();
        do {
            buffer[0] = xrf.read();
            if (millis() - now > 3000) {
                while (1)
                {
                    signal_error();
                }
            }
        } while (buffer[0] == -1);

        // ok we have data, check it
        if (buffer[0] == 'G')
        {

            // OK LAUNCHING & start logger
            DEBUG_LOG_LINE("Launch GO");
            digitalWrite(PIN_MOSFET, HIGH); // Start ignition
            logger();

            while (1);
        }
        else
        {
            DEBUG_LOG_LINE("Error: Bad sync!");
            while (1)
            {
                signal_error();
            }
        }
    }
    else // bad data, error
    {
        DEBUG_LOG_LINE("Error: Bad password!");
        while (1)
        {
            signal_error();
        }
    }
}

