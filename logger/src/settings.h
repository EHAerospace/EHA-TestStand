#ifndef SETTINGS_H
#define SETTINGS_H

#define SERIAL_BAUD_RATE 115200
#define RADIO_BAUD_RATE 9600
#define HANDSAKE_WAIT_MS 3000
#define RECORD_SECONDS 30

// Error constants
#define NO_ERROR 0
#define BLINK_ALL_WORK_DONE 1
#define ERROR_NO_SD_CARD 4
#define ERROR_NO_CONFIRMATION 5
#define ERROR_BAD_SYNC 6
#define ERROR_WRONG_PASSWORD 7
#define ERROR_RADIO_SERIAL 8
#define ERROR_SCALE 9
#define ERROR_BUZZ_FREQUENCY 440  // La
#define NORMAL_BUZZ_FREQUENCY 262 // Do

// Debug macros
#define DEBUG_ACTIVE 1
#if DEBUG_ACTIVE
#define DEBUG_LOG(x) Serial.print(x)
#define DEBUG_LOG_LINE(x) Serial.println(x)
#else
#define DEBUG_LOG(x)
#define DEBUG_LOG_LINE(x)
#endif

// UART port selection for receive commands (for debug)
#define INPUT_COMMANDS_UART 1
#if INPUT_COMMANDS_UART == 0
#define INPUT_COMMANDS() Serial.read()
#elif INPUT_COMMANDS_UART == 1
#define INPUT_COMMANDS() xrf.read()
#endif

#endif
