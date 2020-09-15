#ifndef RADIO_H
#define RADIO_H

int radio()
{
    int i = 0;
    char buffer[16];
    const char *payload = "PAUPER AD ASTRA"; // 15 bytes

    DEBUG_LOG_LINE("Waiting for password... ");
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

    //
    // Password receive and check
    while (i < (int)strlen(payload))
    {
        int val = INPUT_COMMANDS(); // Receive serial data. Macro for debugging options

        if (val == NULL_STRING)
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
    DEBUG_LOG_LINE("Password command received!");

    //
    // send confirmation
    delay(100);
    xrf.print('K');

    //
    // listen for commands and wait with HANDSAKE_WAIT_MS timeout
    DEBUG_LOG("Waiting for command confirmation... ");
    long now = millis();
    do
    {
        buffer[0] = INPUT_COMMANDS(); // Receive serial data. Macro for debugging options
        if (millis() - now > HANDSAKE_WAIT_MS)
        {
            DEBUG_LOG_LINE("Error: No command received");
            return ERROR_NO_COMMAND_RECEIVED;
        }
    } while (buffer[0] == NULL_STRING);

    //
    // check command received
    switch (buffer[0])
    {
    case 'G':
        // OK LAUNCHING
        DEBUG_LOG_LINE("Received command: Launch GO!!");
        digitalWrite(PIN_MOSFET, HIGH); // Start ignition
        delay(30000);
        digitalWrite(PIN_MOSFET, LOW); // Stop ignitor for security reasons
        return NO_ERROR;
        break;

    default:
        DEBUG_LOG_LINE("Error: Received command not recognized");
        return ERROR_COMMAND_NOT_RECOGNIZED;
        break;
    }
}

#endif // RADIO_H
