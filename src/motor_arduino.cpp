#include <HardwareSerial.h>
#include <motor.h>
#include "motor_arduino.h"

#define MOTOR_TYPE ArduinoMotor

MOTOR_IMPLEMENT(void)::init()
{
    servo.attach(_pin);
}

MOTOR_IMPLEMENT(void)::_write(double pos)
{
    //Serial.println("WRITE_SERVO");
    pos=_map(pos, 0, 270, 0, 180);
    servo.write(pos);
}
