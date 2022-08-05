#include <String.h>
#include <Servo.h> 
#include <SPI.h>
#include <Arduino_LSM6DSOX.h>
#include "MBED_RPi_Pico_TimerInterrupt.h"
#include <MadgwickAHRS.h>
#include <StepperController.h>


#define DEBUG false


//Initializing base wheel motors
StepperMotor leftMotor(5, 6);
StepperMotor rightMotor(10, 11); 

//Hardware interrupts
MBED_RPI_PICO_Timer ITimer0(0);
MBED_RPI_PICO_Timer ITimer1(1);

void TimerHandler1(uint alarm_num){
  TIMER_ISR_START(alarm_num);
  leftMotor.timerHandler();
  TIMER_ISR_END(alarm_num);
}

void TimerHandler2(uint alarm_num){
  TIMER_ISR_START(alarm_num);
  rightMotor.timerHandler();
  TIMER_ISR_END(alarm_num);
}

//Variables for Angle Calculation
Madgwick filter;
const float sensorRate = 104.00;
float pitchFilteredOld;
float xAcc, yAcc, zAcc;
float xGyro, yGyro, zGyro;
float roll, pitch, heading;

//PID Constants
float kp = 1/11.0, 
float ki;
float kd = 1/40.0;
float desired_angle = 0;
float error, error_dt;
float pid_p, pid_i, pid_d;
float speed; 
int wallace = 0;

void setup(){
  //setting up 1/16th MICROSTEPPING, should be handled in StepperController class next time
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  pinMode(3, OUTPUT);
  digitalWrite(3, HIGH);
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
  pinMode(7, OUTPUT);
  digitalWrite(7, HIGH);
  pinMode(8, OUTPUT);
  digitalWrite(8, HIGH);
  pinMode(9, OUTPUT);
  digitalWrite(9, HIGH);
  //-------------------------------------------------------
  
  Serial.begin(9600);
  ITimer0.attachInterruptInterval(50, TimerHandler1);
  ITimer1.attachInterruptInterval(50, TimerHandler2);

  while (!Serial);
  if(!IMU.begin())  {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
  filter.begin(sensorRate);
}

void loop() {
  //Collect the data from the IMU and calculate the orientation (angle) of the robot using the MadgwickAHRS Algorithm
  if(IMU.accelerationAvailable() && IMU.gyroscopeAvailable()){
    IMU.readAcceleration(xAcc, yAcc, zAcc);
    IMU.readGyroscope(xGyro, yGyro, zGyro); 
    filter.updateIMU(xGyro, yGyro, zGyro, xAcc, yAcc, zAcc);
    pitch = filter.getRoll();
    float pitchFiltered = 0.1 * pitch + 0.9 * pitchFilteredOld; // low pass filter
    pitchFilteredOld = pitchFiltered;
  }
  #if DEBUG:
    Serial.print("angle: ");
    Serial.println(pitchFilteredOld);
    Serial.print("xGyro: ");
    Serial.println(xGyro);
  #endif

  //Proportional and Differential Control (Integral part yet to be implemented and tuned)
  if (abs(xGyro) < 1){
    xGyro = 0;
  } 
  //Proportional Error
  error = pitchFilteredOld - desired_angle;
  pid_p = error * kp;
  //Differential Error 
  error_dt - xGyro; //xGyro is already the ROC of the angle so we can utilize this then approximating the derivative of the error
  pid_d = kd*error_dt;

  speed = pid_p + pid_d;
  //Serial.println(speed);  

  leftMotor.run(-speed);
  rightMotor.run(speed);
}
