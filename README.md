# EHA-TestStand
This repository contains all the digital resources used for the test stand. This test stand is used to test the rocket boosters and recolect data about the performance it gave.

This repository consists of two parts: the logger and the ploter. The first part is responsable for recording all the data, which is then read by the ploter who is used to visualize the data recolected.

## Logger
The logger is an application that runs on an arduino board, which takes care of all the data that is being recolected by the peripherals and then saves it to a SD card.

For building and debugging the application PlatformIO (referenced as PIO), is being used. This means that this project can be compiled, uploaded and debugged. 

To compile the project, make sure that all the dependencies are installed, to do this go to the [logger](logger/) folder and run `./install_dependencies.sh`. After that simply run the following command: `pio run`. To upload the program to the arduino board, after connecting it with a usb cable, you need to run the next command: `pio run --target upload`. For debugging, just type `pio run --target monitor`, and all the `Serial.print(...)` or `Serial.println(...)` calls will print on the console from which you ran the command.  

### Connected Peripherals
* The most obvious connected peripheral is the weight, which sends all the data to the arduino board via an analog signal. To translate all this analog data the library [HX711](https://github.com/bogde/HX711) is being used. This library is also open source.
* The second peripheral is takes care of the SD card. This is used to store all the data on a microSD card with an arduino module.

### External Dependencies
* **HX711**: This library is being used to translate the weights' analog signals into digital data that then can be stored into a file. The version of the library being used is included on this repository. This dependency is already included inside [this folder](logger/Dependencies).
* Other dependencies such as the SD card reader need to be installed with PlatformIO. A little script is included to add all these dependencies.

## Graph

### Dependencies
All the dependencies for this python project can be installed with pip.
* matplotlib
* glob
* argparse
