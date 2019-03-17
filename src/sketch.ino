#include "arduino_config.h"
#include "robot_arduino.h"
#include "motor_arduino.h"
#include "communication_arduino.h"
#include <HardwareSerial.h>
#include "arduino_tools.h"

//Robot * robot;

//typedef BaseRobot<ArduinoRobot, ArduinoMotor, ArduinoCommunicator> Robot;
Robot * robot;





void setup()
{
    Serial.begin(57600);
    //Serial2.begin(57600);
    //Serial.println("Setup start");
  
    //robot=new Robot();
     
     robot=new Robot();
     //robot->write_config_to_storage();
     //robot->start_time=millis();
     //Serial.println("Started");
     //r->restart();
     //r->read_config_from_storage();
    //Serial.println("Setup done");
    
}

void loop()
{
    robot->main_loop();
   //robot.communicator.update();
}
