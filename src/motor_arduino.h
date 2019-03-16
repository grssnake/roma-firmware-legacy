#ifndef MOTOR_ARDUINO_H
#define MOTOR_ARDUINO_H
#include <Multiservo.h>

class ArduinoMotor
{
public:
    ArduinoMotor()
    {
        //Serial.println("ARDUINO_MOTOR");
    }
    Multiservo servo;
};
#endif





