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

#define PIN_ACTIVATION_BUTTON 4				// Activation button pin
#define PIN_LED 5
#define PIN_BUZZ 6
#define PIN_IGNITER 7	 			// Ignition power stage pin
#define PIN_POWER 8
#define PIN_CLOCK	2				// Clock pin
#define PIN_DATAOUT	3	 			// Data output pin
#define BUZZ_FREQUENCY 440

// Part of the SD pins
#define PIN_SDCS 10

void setup_peripherals()
{
  // Setup the Serial
  Serial.begin(38400);
  while (!Serial);
  Serial.print("Serial initialized.");

  // Setup SD card
  Serial.print("Initializing SD card...");
  if (!SD.begin(PIN_SDCS))
  {
    Serial.println(" Initialization failed!");
    return;
  }
  Serial.println("Done.");

  // Setup pins
  Serial.print("Setting pin modes...");
  pinMode(PIN_ACTIVATION_BUTTON, INPUT);
  pinMode(PIN_IGNITER, OUTPUT);
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
    tone(PIN_BUZZ, BUZZ_FREQUENCY, 500);
    digitalWrite(PIN_LED, LOW);
    noTone(PIN_BUZZ);
    delay(interval);
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
  // Initializes HX711
  HX711 scale(PIN_DATAOUT, PIN_CLOCK);

  setup_peripherals();

  Serial.println("Put the nozzle looking upwards; Introduce the the ignite into the nozzle. Danger Zone!!");
  Serial.println("Waiting 3 seconds..");
  delay(3000);

  // Prepare the scale and log the calculated scale factor to the serial
  Serial.print("Calibrating sensor...");
  scale.set_scale();
  scale.tare();      //Reset the scale to
  long zeroFactor = scale.read_average(); //Get a baseline reading
  
  Serial.println(" Done.");
  Serial.print("ReferenceValue = "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
  Serial.println(zeroFactor);

  make_signals(10, 500);

  Serial.println();
  Serial.print("Creating log file...");

  // Create the file and add the header
  File file = SD.open("data.txt", FILE_WRITE);
  file.print("Tare: ");
  file.println(zeroFactor);
  file.print("Starting timestamp: ");
  file.println(millis());
  file.println("From lecture position:");
  Serial.println(" Done.");

  // Take samples of the sensor and save it on the SD card
  Serial.println("Start!");
  take_samples(scale, file, 800);

  // Closes the file:
  file.print("Final timestamp: ");
  file.println(millis());
  file.close();
  Serial.print("Program finalized");
}

void loop()
{
}
