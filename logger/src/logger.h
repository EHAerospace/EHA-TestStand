#ifndef LOGGER_H
#define LOGGER_H

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
}


#endif // LOGGER_H
