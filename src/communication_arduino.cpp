#include "arduino_config.h"
#include <robot.h>
#include "communication_arduino.h"
#include <communication.h>
#include <HardwareSerial.h>


#define COMM_TYPE ArduinoCommunicator


#define _IMPL_PRINT(t)  template<> COMM_IMPLEMENT(void)::print<t>(t n) {serial.print(n);}
#define IMPL_PRINTLN(t)  template<> COMM_IMPLEMENT(void)::println<t>(t n) {serial.println(n);}

#define IMPL_PRINT(t) _IMPL_PRINT(t) IMPL_PRINTLN(t)

COMM_IMPLEMENT(void)::init()
{
}
COMM_IMPLEMENT(unsigned)::available()
{
    return serial.available();
}

COMM_IMPLEMENT(int)::read()
{
    return serial.read();
}
#ifndef TEXT_PROTOCOL
COMM_IMPLEMENT(void)::write(unsigned char b)
{
    serial.write(b);
}

COMM_IMPLEMENT(void)::write(unsigned char * data, unsigned long length)
{
    for (unsigned long i=0;i<length;i++, data++)
        write(*data);
}
#else
IMPL_PRINT(int)
IMPL_PRINT(unsigned char)
IMPL_PRINT(char)
IMPL_PRINT(unsigned)
IMPL_PRINT(long)
IMPL_PRINT(unsigned long)

IMPL_PRINT(char *)
IMPL_PRINT(const char *)
IMPL_PRINT(bool)

COMM_IMPLEMENT(void)::println()
{
    serial.println("");
}
#endif


