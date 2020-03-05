

void led_and_buzz_error(int signal_count, bool buzz_on)
{
	while (true)
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
}

void led_and_buzz_normal(bool led_on_off, bool buzz_on_off)
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
