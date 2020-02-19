#ifndef INIT_H
#define INIT_H

int setup_peripherals()
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
		return ERROR_NO_SD_CARD;
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


int init() {	
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

    int res = setup_peripherals();
    if(res != 0)
	    return res;

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

#endif // INIT_H
