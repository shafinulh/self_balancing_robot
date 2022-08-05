#ifndef StepperController
#define StepperController
#include <Arduino.h>
#include <String.h>
#include <Servo.h> 

class Utils{
public: 
  static double map(double min1, double max1, double min2, double max2, double val);
  static double bound(double val, double min, double max);
};

class StepperMotor{
//-----------------------------------------------------------------------------------------------------------
//Variable Declaration
private:
    int _counter; 
    int _update_period;
    int _step_pin;
    int _dir_pin; 
    int _position;
    int _direction; 
    int _last_step_time;
    bool _step_direction;  
    double _speed;
    int _delay;
//-------------------------------------------------------------------------------------------------------------
//Function Declarations
public: 
    StepperMotor(int step_pin, int dir_pin);
    void run(double speed);
    void update();
    void timerHandler();
    double getPosition();
    void printDelay();
    void step();
};
