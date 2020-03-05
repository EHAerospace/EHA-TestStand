#ifndef RADIO_H
#define RADIO_H

int radio()
{
    int i = 0;
    char buffer[16];
    const char *payload = "PAUPER AD ASTRA"; // 15 bytes

    DEBUG_LOG_LINE("Waiting for radio command... ");
    //    do
    //    {
    //        // DEBUG_LOG_LINE(xrf.read());
    //        buffer[0] = xrf.read();
    //    } while (buffer[0] != 'P');
    //
    //    DEBUG_LOG("received char: ");
    //    DEBUG_LOG(buffer[0]);
    //    DEBUG_LOG(" -- password char: ");
    //    DEBUG_LOG_LINE(payload[0]);
    //
    // ok we have data, check it
    // wait for answer with 1 sec timeout

    while (i < 15)
    {
        int val = INPUT_COMMANDS(); // Macro for debugging options

        if (val == -1)
            continue;

        DEBUG_LOG("received char: ");
        DEBUG_LOG((char)val);
        DEBUG_LOG(" -- password char: ");
        DEBUG_LOG_LINE(payload[i]);

        if (val == payload[i])
        {
            i++;
            continue;
        }
        else
        {
            i = 0;
            DEBUG_LOG_LINE("Bad password! Restarting password exchange.");
            // return ERROR_WRONG_PASSWORD;
        }
    }

    // if (strncmp(buffer+8, payload, 15) == 0)
    DEBUG_LOG_LINE("Launch command received!");

    // send confirmation
    delay(100);
    xrf.print('K');

    // listen
    DEBUG_LOG("Waiting for radio command confirmation... ");
    // wait for answer with HANDSAKE_WAIT_MS timeout
    long now = millis();
    do
    {
        buffer[0] = INPUT_COMMANDS(); // Macro for debugging options
        if (millis() - now > HANDSAKE_WAIT_MS)
        {
            DEBUG_LOG_LINE("Error: No confirmation received");
            return ERROR_NO_CONFIRMATION;
        }
    } while (buffer[0] == -1);

    // ok we have data, check it
    switch (buffer[0])
    {
    case 'G':
        // OK LAUNCHING
        DEBUG_LOG_LINE("Launch GO!!");
        digitalWrite(PIN_MOSFET, HIGH); // Start ignition
        return 0;
        break;

    default:
        DEBUG_LOG_LINE("Error: Bad confirmation received");
        return ERROR_BAD_SYNC;
        break;
    }
}

#endif // RADIO_H
