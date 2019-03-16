#include "Ultrasound.h"

UltrasoundSensor::UltrasoundSensor(int _trigger_pin, int _echo_pin){
    trigger_pin = _trigger_pin;
    echo_pin = _echo_pin;        
    
    pinMode(trigger_pin, OUTPUT);
    pinMode(echo_pin, INPUT);        
}

void UltrasoundSensor::ping(){      
  digitalWrite(trigger_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger_pin, LOW);
  attachInterrupt(digitalPinToInterrupt(echo_pin), ultrasound_sensor_interrupt_handler, FALLING);      
  start = micros();
  
}
void UltrasoundSensor::stop(){
  detachInterrupt(digitalPinToInterrupt(echo_pin));
}


UltrasoundSensorManager::UltrasoundSensorManager(){
    // echo pins could be 2, 3, 18, 19, 20, 21
    sensors[0] = UltrasoundSensor(4, 2);
    sensors[1] = UltrasoundSensor(5, 3);
    sensors[2] = UltrasoundSensor(17, 20);
    ready = true;
}

void UltrasoundSensorManager::ping(){
    ready = false;
    sensors[current_sensor_id].ping();        
}
void UltrasoundSensorManager::next(){
    current_sensor_id ++;
    if (current_sensor_id >=3)
        current_sensor_id = 0;
    ready = true;
}

bool UltrasoundSensorManager::timeout(){
    return sensors[current_sensor_id].start < micros() - 39000;        
  

}

void UltrasoundSensorManager::set_null(){
    sensors[current_sensor_id].stop();
    sensors[current_sensor_id].last_value = 0;        
}


UltrasoundSensorManager * us_manager;

void ultrasound_sensor_interrupt_handler(){
    UltrasoundSensor & sensor = us_manager->sensors[us_manager->current_sensor_id];
    sensor.last_value = micros() - sensor.start;
    us_manager->next();    
}

