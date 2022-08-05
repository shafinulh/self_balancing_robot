#include <String.h>
#include <Servo.h> 
#include <SPI.h>
#include <Arduino_LSM6DSOX.h>
#include "MBED_RPi_Pico_TimerInterrupt.h"
#include <MadgwickAHRS.h>
#include <StepperController.h>

//7,8,9
StepperMotor leftMotor(5, 6);
//4,3,2
StepperMotor rightMotor(10, 11); 

MBED_RPI_PICO_Timer ITimer0(0);
MBED_RPI_PICO_Timer ITimer1(1);

void TimerHandler1(uint alarm_num){
  TIMER_ISR_START(alarm_num);
  leftMotor.timerHandler();
  //rightMotor.timerHandler();
  TIMER_ISR_END(alarm_num);
}

void TimerHandler2(uint alarm_num){
  TIMER_ISR_START(alarm_num);
  //leftMotor.timerHandler();
  rightMotor.timerHandler();
  TIMER_ISR_END(alarm_num);
}

Madgwick filter;
const float sensorRate = 104.00;
float pitchFilteredOld;
float xAcc, yAcc, zAcc;
float xGyro, yGyro, zGyro;
float roll, pitch, heading;

double speed_p = 0, speed_i = 0, speed_d = 0;
double kp, ki, kd;
double speed; 
int wallace = 0;

void setup(){
  ////////////////////////setting up MICROSTEPPING
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
  /////////////////////////
  
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
  if(IMU.accelerationAvailable() && IMU.gyroscopeAvailable()){
    IMU.readAcceleration(xAcc, yAcc, zAcc);
    IMU.readGyroscope(xGyro, yGyro, zGyro); 
    filter.updateIMU(xGyro, yGyro, zGyro, xAcc, yAcc, zAcc);
    pitch = filter.getRoll();
    float pitchFiltered = 0.1 * pitch + 0.9 * pitchFilteredOld; // low pass filter
    //Serial.println("pitch: " + String(pitchFiltered));
    pitchFilteredOld = pitchFiltered;
  }
  // Serial.print("angle: ");
  // Serial.println(pitchFilteredOld);
  Serial.print("xGyro: ");
  Serial.println(xGyro);

  // speed_p = (pitchFilteredOld/(90.0*10))*18; //ONLY PROPORTIONAL CONTROL
  if (abs(xGyro) < 1){
    xGyro = 0;
  } 
  speed_p = (pitchFilteredOld) * (1/11.0); 
  speed_d = (1/40.0) * xGyro;
  speed = 1*speed_p + 0*speed_d;
  if (speed == 0){
    speed = 0.00001;
  }

  leftMotor.run(-speed);
  rightMotor.run(speed);

}
