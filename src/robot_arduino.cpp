#include "arduino_config.h"
#include "arduino_tools.h"
#include "robot.h"
#include "robot_arduino.h"
#include "motor_arduino.h"
#include "communication_arduino.h"
#include <HardwareSerial.h>
#include <avr/pgmspace.h>

#define ROBOT_TYPE ArduinoRobot
#define MOTOR_TYPE ArduinoMotor
#define COMM_TYPE ArduinoCommunicator
#define BASE_ROBOT BaseRobot<ROBOT_TYPE, MOTOR_TYPE, COMM_TYPE>





ROBOT_BASE_IMPLEMENT(void)::read_config_from_storage()
{
    EEPROM_READ(sizeof(unsigned long),config);
}

ROBOT_BASE_IMPLEMENT(unsigned long)::get_config_crc_from_storage()
{
    unsigned long result=0;
    EEPROM_READ(0,result);
    return result;
}

ROBOT_BASE_IMPLEMENT(bool)::write_config_to_storage()
{
    unsigned long crc=OBJECT_CRC(config);
    EEPROM_UPDATE(0,crc);
    return EEPROM_UPDATE(sizeof(unsigned long),config);
}

ROBOT_BASE_IMPLEMENT(bool)::validate_config()
{
    read_config_from_storage();
    return get_config_crc_from_storage() == OBJECT_CRC(config);
}

ROBOT_BASE_IMPLEMENT(bool)::break_storage_config()
{
    unsigned crc_size=sizeof(unsigned long);
    unsigned address=random(crc_size+sizeof(config));
    return EEPROM_put(address, (unsigned char)random(255));
}




ROBOT_BASE_IMPLEMENT(unsigned long)::_millis()
{
      return millis();
}

ROBOT_BASE_IMPLEMENT(void)::restart()
{
    Serial.println("go to restart");
    pinMode(reset_pin, OUTPUT);
    for (int i=5;i>=0;i--)
    {
        Serial.println(i);
        delay(1000);
    }
    digitalWrite(reset_pin, LOW);
}

ROBOT_BASE_IMPLEMENT(void)::init_sound()
{
    //Serial1.begin (9600);
   // mp3_set_serial (Serial1);
    delay(1);
    //mp3_set_volume (30);
}

ROBOT_BASE_IMPLEMENT(void)::play_sound(const char * string)
{
     mp3_play (1);
}

ROBOT_BASE_IMPLEMENT(float)::get_sensor_data(int i)
{ 
    return 0;
}
ROBOT_BASE_IMPLEMENT(float)::get_us_sensor_data(int i) 
{
     return 0;
}
ROBOT_BASE_IMPLEMENT(void)::stop_sound()
{
    mp3_stop();
}

ROBOT_BASE_IMPLEMENT(void)::update_sensors()
{
}


ROBOT_BASE_IMPLEMENT(void):: start_iteration()
{
    
    current_ms = millis();  
    iteration_started_mcs = micros();    
}



ROBOT_BASE_IMPLEMENT(void):: finish_iteration()
{
    last_loop_mcs = micros() - iteration_started_mcs;
}

ROBOT_BASE_IMPLEMENT(unsigned):: get_available_ram()
{
    extern int __heap_start, *__brkval; 
    int v; 
    return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);   
}

ROBOT_BASE_IMPLEMENT(void):: acc_values(float * values)
{
    int * result = gyroacc.readFromAccelerometer();
    values[0] = result[0];    
    values[1] = result[1];    
    values[2] = result[2]; 
}

ROBOT_BASE_IMPLEMENT(void )::compass_values(float * values)
{
    int * result = gyroacc.readFromCompass();
    values[0] = result[0];    
    values[1] = result[1];    
    values[2] = result[2];    
}

ROBOT_BASE_IMPLEMENT(void):: gyro_values(float * values)
{
    int * result = gyroacc.readFromAccelerometer();
    values[0] = result[0]; // x   
    values[1] = result[1]; // y   
    values[2] = result[2]; // z
    values[3] = result[3]; // w
}
/*
void Robot::start_iteration()
{
    BASE_ROBOT::start_iteration();
}*/

/*
ROBOT_BASE_IMPLEMENT(void):: update_ultrasound_sensors()
{
    if (ultrasound_sensor_manager->ready)
        ultrasound_sensor_manager->ping();
    else
    if (ultrasound_sensor_manager->timeout()) 
    {
        ultrasound_sensor_manager->set_null();
        ultrasound_sensor_manager->next();
        ultrasound_sensor_manager->ping();
    }
}
ROBOT_BASE_IMPLEMENT(void):: set_us_manager(UltrasoundSensorManager * manager)
{
    this->ultrasound_sensor_manager = manager;
}*/

void Robot::main_loop()
{
    if (!session_opened)
    {
        wait_for_session();
        return;
    }
    
    start_iteration();
    
    //Serial.println(current_initialization_step);
    if (special_state)
    {
        handle_special_state();
    }
    else
    {
        //listen_for_command();
        perform_outcome_stream();
        update_motors();
    }
    listen_for_command();
    finish_iteration();
}    
    
    

    
    

void Robot::wait_for_serial(HardwareSerial & serial)
{
     
    if (!session_opened)
    {
        
        if (serial.available())
        { 
            CLI cli;
            bool stop=false;
            while(!stop)
            {    
                while(!serial.available()) {delay(1);}
                stop=cli.feed(serial.read());
            }
            if (stop)
            {
                if (!strcmp(cli.args[0], "knock"))
                {
                    communicator.set_serial(serial);
                    perform_knock();
                    session_opened=true;
                    if (!initialized)
                    {
                        init_safe_elements();
                        
                    }
                    handle_special_state();
                    free_serial_income_buffer(serial);
                    return;
                    
                }
            }
            
        }
        /*
        if (serial.available()>=5 && serial.available()<=8)
        {
            
            char knock[10]={0};
            knock[5]=0;
            char * k=knock;
            while(serial.available())
            {
                *k=serial.read();
                k++;
                if (!strcmp(knock, "knock"))
                {

                }
                delay(10);  
            }
            free_serial_income_buffer(serial);
        }
        */
    }
}

void Robot::wait_for_session()
{
    
    wait_for_serial(Serial);
    //wait_for_serial(Serial2);
}



