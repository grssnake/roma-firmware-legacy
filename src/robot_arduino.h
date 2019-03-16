#ifndef ROBOT_ARDUINO_H
#define ROBOT_ARDUINO_H
#include <HardwareSerial.h>
#include <Wire.h>
#include <DFPlayer_Mini_Mp3.h>
#include "Ultrasound.h"
#include "GY_85.h"
#include <robot.h>
#include "motor_arduino.h"
#include "communication_arduino.h"

#define BASE_ROBOT BaseRobot<ArduinoRobot, ArduinoMotor, ArduinoCommunicator>



class ArduinoRobot
{
protected:
    unsigned char current_serial_count=0; 
    //HardwareSerial * serials[MAX_SERIALS];
    unsigned long iteration_started_mcs = 0;
public:
    bool session_opened=false;    
    
    unsigned long start_time=0;
    bool communication_mode_selected=false;
    GY_85 gyroacc;
    UltrasoundSensorManager * ultrasound_sensor_manager;
    unsigned long int current_ms = 0;
    volatile unsigned int current_sensor_data[8];
    volatile int current_sensor_id = 0;
    
    
};
class Robot:public BASE_ROBOT
{
public:
    Robot():BASE_ROBOT()
    {
        digitalWrite(reset_pin, HIGH);
    }
    void main_loop();
    void wait_for_session();
    void wait_for_serial(HardwareSerial & serial);
};

#endif
