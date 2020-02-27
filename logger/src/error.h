#define BLINKS_BTWN_INIT_PHASES 1
#define ERROR_NO_SD_CARD 4
#define ERROR_NO_CONFIRMATION 5
#define ERROR_BAD_SYNC 6
#define ERROR_WRONG_PASSWORD 7
#define ERROR_RADIO_SERIAL 8
#define ERROR_SCALE 9
#define ALL_WORK_DONE 200
#define ERROR_BUZZ_FREQUENCY 440  // La
#define NORMAL_BUZZ_FREQUENCY 262 // Do

void blink_led_and_buzz_error(int signal_count, bool buzz_on)
{
	for (int f = 0; f < signal_count; f++)
	{
		digitalWrite(PIN_LED, HIGH);
		if (buzz_on)
		{
			tone(PIN_BUZZ, ERROR_BUZZ_FREQUENCY, 500);
		}
		delay(500);
		digitalWrite(PIN_LED, LOW);
		if (buzz_on)
		{
			noTone(PIN_BUZZ);
		}
		delay(500);
	}
	delay(1000);
}

void blink_led_and_buzz_normal(bool led_on_off, bool buzz_on_off)
{
	if (led_on_off)
	{
		digitalWrite(PIN_LED, HIGH);
	}
	else
	{
		digitalWrite(PIN_LED, LOW);
	}

	if (buzz_on_off)
	{
		tone(PIN_BUZZ, NORMAL_BUZZ_FREQUENCY);
	}
	else
	{
		noTone(PIN_BUZZ);
	}
}
