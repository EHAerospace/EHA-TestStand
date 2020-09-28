# Documentation for the EHA test stand
The EHA test stand is a test stand that is meant to be used as a testing ground for rocket engines. This consists of various software projects, and the physical structure of course.

* **Graph**: this project is used to visualize the data given by the logger project. This is just a software project, so doesnt need any specific hardware to use, just python and the dependencies that are going to be shown later.
* **Logger**: this project is used to log all the data into a file inside the SD card. This project is more complex than the grapher, as is not just software, it also needs to be flushed into an arduino with specific peripherals and the structure to hold the engine when is turned on.

## Graph

## Logger
The logger is an application that runs on a arduino board, and takes care about all the data that is being recolected by the peripherals and then saved on an SD card. 

### How to build
For building and debugging the softwsare, PlatformIO (referenced as PIO) is being used. This means that this project can be compiled, uploaded and debugged using this tool. 

To compile the project, make sure that all the dependencies are installed, to do this go to the logger folder and run `./install_dependencies.sh`. After that simply run the following command: `pio run`. To upload the program to the arduino board, after connecting it with a usb cable, you need to run the next command: `pio run --target upload`. For debugging, just type `pio run --target monitor`, and all the `Serial.print(...)` or `Serial.println(...)` calls will print on the console you ran the command.

Some error might occur when trying to upload the code to the arduino board as the application might not have all the permissions to write through the serial port. To fix this on a linux system, your user needs to be added to the group dialout. To add your user to that group just type the next command: `usermod -a -G dialout youruser`.

There is two types of builds you can do, the debug and release one: the debug build is meant to, as the name implies, to debug the application when errors occur, making the development faster; in the other hand, the release build is meant to be used as a final product with faster performance. Both of this builds are available on the software by changing some defines under the [settings.h](logger/src/settings.h) file. If the `DEBUG_ACTIVE` define is set to 1 `DEBUG_LOG` and `DEBUG_LOG_LINE` print the specified string through the UART; if `DEBUG_ACTIVE` is set to 0, both of the functions do nothing, speeding up the software.
