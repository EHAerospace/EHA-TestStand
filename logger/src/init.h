#ifndef INIT_H
#define INIT_H

String get_filename()
{
        unsigned int i = 0;
        while (SD.exists(String("data") + String(i) + String(".txt")))
        {
                i++;
        }
        DEBUG_LOG_LINE(String("filename: data") + String(i));
        return String("data") + String(i) + String(".txt");
}

int setup_peripherals()
{
        // Setup the Serial
        Serial.begin(SERIAL_BAUD_RATE);
        while (!Serial)
                ;
        DEBUG_LOG_LINE("Serial initialized.");
        DEBUG_LOG_LINE("EH Launcher ON");

        // Setup GPIO pins
        DEBUG_LOG("Setting GPIO pins...");
        pinMode(PIN_LED, OUTPUT);
        pinMode(PIN_MOSFET, OUTPUT);
        digitalWrite(PIN_MOSFET, LOW);
        pinMode(PIN_BUZZ, OUTPUT);
        noTone(PIN_BUZZ);
        DEBUG_LOG_LINE(" Done.");
        blink_led_and_buzz_normal(true, true);
        
        // Serial for xrf radio
        DEBUG_LOG("Setting radio module...");
        xrf.begin(RADIO_BAUD_RATE);
        if (!xrf.isListening())
        {
                DEBUG_LOG_LINE("Radio serial port Failed!");
                //return ERROR_RADIO_SERIAL;
        }
        DEBUG_LOG_LINE(" Done.");

        // Prepare the scale and log the calculated scale factor to the serial
        DEBUG_LOG_LINE("Calibrating sensor...");
        if (!scale.is_ready())
        {
                DEBUG_LOG_LINE("Scale sensor Failed!");
                return ERROR_SCALE;
        }
        scale.set_scale();
        scale.tare();                       //Reset the scale to
        zero_factor = scale.read_average(); //Get a baseline reading
        DEBUG_LOG(" Done. ");
        DEBUG_LOG("ReferenceValue = "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
        DEBUG_LOG_LINE(zero_factor);

        // Setup SD card
        DEBUG_LOG("Initializing SD card...");
        if (!SD.begin(PIN_SDCS))
        {
                DEBUG_LOG_LINE("SD Failed!");
                return ERROR_NO_SD_CARD;
        }
        DEBUG_LOG_LINE("Done.");
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

        return 0;
}

#endif // INIT_H
