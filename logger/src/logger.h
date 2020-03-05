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
	//
	// Take the specified samples asynchronously
	DEBUG_LOG_LINE("Starting sampling routine");
	take_samples(scale, logger_file, 80*RECORD_SECONDS);

	//
	// Closes the file:
	logger_file.print("Final timestamp: ");
	logger_file.println(millis());
	logger_file.close();
	DEBUG_LOG("Program finalized"); 
}


#endif // LOGGER_H
