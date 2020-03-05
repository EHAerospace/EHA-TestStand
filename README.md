# EHA-TestStand
This repository contains all the digital resources used for the test stand. This test stand is used to test the rocket boosters and recolect data about the performance it gave.

This repository consists of two little projects: the logger and the ploter. The first project is the responsable for taking all the data and the second one is used to visualize the data recolected.

## Logger
The logger is an application that runs on a arduino board, and takes care about all the data that is being recolected by the peripherals and then saved on an SD card.

For building and debugging the application PlatformIO (referenced as PIO) is being used. This means that this project can be compiled, uploaded and debugged. 

To compile the project, simply go inside the [logger](logger/) folder and run the following command: `pio run`. To upload the program to the arduino board, after connecting it with a usb cable, you need to run the next command: `pio run --target upload`. For debugging, just type `pio run --target monitor`, and all the `Serial.print(...)` or `Serial.println(...)` calls will print on the console you ran the command.  

### Connected Peripherals
* The most obvious connected peripheral is a scale, which sends all the data to the arduino board via an analog signal. To translate all this analog data the library [HX711](https://github.com/bogde/HX711) is being used. This library is also open source.
* The second peripheral is taking care of the SD card. This is used to store all the data on a microSD card with a arduino module.

### External Dependencies
* **HX711**: This library is being used to translate the scales analog signals into a digital data that then can be stored. The version of the library that is being used is included on this repository. This dependency is already included inside [this folder](logger/Dependencies).
* Other dependencies such as the SD card reader need to be installed with PlatformIO. A little script is included to add all these dependencies.

## Graph

### Dependencies
All the dependencies for this pyhton project can be installed with pip.
* matplotlib
* glob
