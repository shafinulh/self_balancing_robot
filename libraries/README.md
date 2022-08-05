# About
This directory is intended for required project header files

## StepperController
A custom built library to control the NEMA-17 Stepper Motors for the base of the Self Balancing Robot. 

As this was my first time writing the software for a robotic system, I chose to create a custom library to gain experience with writing basic robot controls and coding in C++ in general. Additionally, writing my own library means I have full control over it so I can design it the way I want (every codebase I go look at makes different design choices and I am excited to discover more optimal approaches in the future). It's also nice having something that I can easily build on and integrate into future projects, as well as the understanding all the code that I write/use.

## Arduino_LSM6DSOX.h
A pre-built library that allows use of the inertial measurement unit (IMU) that is on the Arduino Nano RP2040 Connect board.

## MadgwickAHRS
A pre-built library that implements the MadgwickAHRS algorithm to get the angle and orientation of an object based on accelerometer and gyroscope readings.

## MBED_RPI_PICO_TimerInterrupt
A pre-built library to use interrupts from Hardware Timers on the Arduino Nano RP2040 Connect
