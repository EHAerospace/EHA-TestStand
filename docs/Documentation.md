# Documentation for the EHA test stand
The EHA test stand is a test stand that is meant to be used as a testing ground for rocket engines. This consists of various software projects, and the physical structure of course.

* **The grapher**: this project is used to visualize the data given by the logger project. This is just a software project, so doesnt need any specific hardware to use, just python and the dependencies that are going to be shown later.
* **The logger**: this project is used to log all the data into a file inside the SD card. This project is more complex than the grapher, as is not just software, it also needs to be flushed into an arduino with specific peripherals and the structure to hold the engine when is turned on.