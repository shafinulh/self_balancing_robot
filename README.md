# self_balancing_robot
Control code for an 2-wheeled self balancing robot.

## Arduino Prototype
<img src="img\sbr_cad.png" height="400" />
<img src="img\sbr_cropped.png" height="400" />
* 2 NEMA 17 Stepper Motors for the robot drive base. Controlled with A4988 motor driver capable of 1/16th microstepping
* Real-time motor control using Arduino Nano RP2040 Connect onboard IMU
* Featured an adjustable chassis to allow the center of mass (CoM) to be shifted which eased the process of tuning the PID controller. 

## Balancing the Robot
* Implemented a PID Control Loop to control the motors based off of the readings from the sensor to help maintain its balance by driving in the direction that it is falling
* Utilized Hardware Timers and Interrupts to continuously read the data from the IMU, calculate the robot's current angle, and step the motors at the necessary speed