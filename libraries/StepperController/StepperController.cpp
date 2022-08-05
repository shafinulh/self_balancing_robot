//-----------------------------------------------------------------
//Header Files
#include <StepperController.h>
#include <Utils.h>
#include <Arduino.h>
#include <String.h>
#include <Servo.h> 
 
//-----------------------------------------------------------------
// Deinitions
#define _MIN_DELAY = 50; //delay for speed 1, they are now very low because we are able to control the motor WAY more precisely 
#define _MAX_DELAY = 600; //delay for speed 0, increase this is minimize lowest speed 

//-----------------------------------------------------------------
StepperMotor::StepperMotor(int step_pin, int dir_pin){
    _step_pin = step_pin; 
    _dir_pin = dir_pin; 
    pinMode(_step_pin, OUTPUT);
    pinMode(_dir_pin, OUTPUT);
    _counter = 0; 
    _update_period = 50;
    _position = 0;
    _last_step_time = 0; 
    _step_direction = 1;
    _speed = 0;
    _delay = _MIN_DELAY;
}
//speed from -1 to 1 
void StepperMotor::run(double speed){
    _speed = Utils::bound(speed, -1, 1);
    _direction = (speed < 0) ? 0 : 1;
    digitalWrite(_dir_pin, _direction);
    _delay = Utils::map(_MAX_DELAY, _MIN_DELAY, 0, 1, abs(_speed));
}

void StepperMotor::update(){
    digitalWrite(_dir_pin, _direction);
    if (micros() - _last_step_time < _delay) return; 
    if (_speed == 0) return;
    _last_step_time = micros(); 
    _step_direction = !_step_direction;
    digitalWrite(_step_pin, _step_direction);
    _position += (_direction) ? 1 : -1;
}
void StepperMotor::timerHandler() { //to handle interrupts
    _counter += _update_period;
    if (_counter < _delay) return;
    _counter = 0;
    step();
}
double StepperMotor::getPosition(){
    return _position;  
}
void StepperMotor::Debug(){
    Serial.println((String)_delay + " " + _direction + " " + micros() + " " + _step_direction);
}
void StepperMotor::step() {
    digitalWrite(_step_pin, !digitalRead(_step_pin));
}