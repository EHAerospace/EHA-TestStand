#ifndef RADIO_H
#define RADIO_H

int radio()
{
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
    int i = 0;

    while (i < 15)
    {
        char val = xrf.read();

        if (val == -1)
            continue;


        DEBUG_LOG("received char: ");
        DEBUG_LOG(val);
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
        }
    }

    // if (strncmp(buffer+8, payload, 15) == 0)
    if (true)
    {
        // send confirmation
        delay(100);
        xrf.print('K');

        DEBUG_LOG_LINE("Launch ORDER");
        // listen
        DEBUG_LOG_LINE("Launch OK LSTN GO");
        // wait for answer with 1 sec timeout
        long now = millis();
        do
        {
            buffer[0] = xrf.read();
            if (millis() - now > 3000)
            {
                return ERROR_NO_CONFIRMATION;
            }
        } while (buffer[0] == -1);

        // ok we have data, check it
        if (buffer[0] == 'G')
        {

            // OK LAUNCHING & start logger
            DEBUG_LOG_LINE("Launch GO");
            digitalWrite(PIN_MOSFET, HIGH); // Start ignition
                                            //logger();
            return 0;
        }
        else
        {
            DEBUG_LOG_LINE("Error: Bad sync!");
            return ERROR_BAD_SYNC;
        }
    }
    else // bad data, error
    {
        DEBUG_LOG_LINE("Error: Bad password!");
        return ERROR_WRONG_PASSWORD;
    }
}

#endif // RADIO_H
