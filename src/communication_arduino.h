#ifndef COMMUNICATION_ARDUINO_H
#define COMMUNICATION_ARDUINO_H
#include "arduino_config.h"
#include <HardwareSerial.h>

#define IMPLEMENT_COMM_MEMBER(impl) \
    Communicator<impl> 

#define COMM_IMPLEMENT(type) \
    template <> \
   type IMPLEMENT_COMM_MEMBER(COMM_TYPE)

class ArduinoCommunicator
{
protected:
    HardwareSerial * seriall;
    HardwareSerial & serial;
    
public:
    ArduinoCommunicator():serial(Serial)
    {
    }
    void set_seriall(HardwareSerial * serial)
    {
        this->seriall=serial;
    }
    void set_serial(HardwareSerial & serial)
    {
        this->serial=serial;
    }
    //inline void print(int n) {serial->print(n);}
    //inline void print(const char * string) {serial->print(string);}
    //inline void println(int n) {serial->println(n);}
    //inline void println(const char * string){serial->println(string);} 
};
#endif
