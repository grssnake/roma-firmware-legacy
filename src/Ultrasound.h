#include <Arduino.h>

#define digitalPinToInterrupt(p) ((p) == 2 ? 0 : ((p) == 3 ? 1 : ((p) >= 18 && (p) <= 21 ? 23 - (p) : -1)))

void ultrasound_sensor_interrupt_handler();

class UltrasoundSensor{
      
    public:
    int trigger_pin;
    int echo_pin;
    unsigned long int last_value = 0;
    unsigned long int start = 0;
    UltrasoundSensor(){
    }
    
    UltrasoundSensor(int _trigger_pin, int _echo_pin);
    
    void ping();
    void stop();
};

class UltrasoundSensorManager{
    public:
    int current_sensor_id = 0;
    bool ready = false;
    UltrasoundSensor sensors [3];
    UltrasoundSensorManager();
    
    
    void ping();
    void next();
    
    bool timeout();
    
    void set_null();
};

extern UltrasoundSensorManager * us_manager;



