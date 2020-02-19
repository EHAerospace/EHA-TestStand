#ifndef LOGGER_H
#define LOGGER_H

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
	signal_ok();
}


#endif // LOGGER_H
