#ifndef MOTOR_H
#define MOTOR_H
#include "config.h"
#include <HardwareSerial.h>

#define IMPLEMENT_MOTOR_MEMBER(impl) \
    Motor<impl> 

#define MOTOR_IMPLEMENT(type) \
    template <> \
   type IMPLEMENT_MOTOR_MEMBER(MOTOR_TYPE)

//double _map(double x, double in_min, double in_max, double out_min, double out_max);
double _map(double x, double in_min, double in_max, double out_min, double out_max);

struct Segment{
  bool active;
  unsigned long int start, finish;
  int begin, end;
  inline  Segment() : active(false) {};
};

#define UNKNOWN -1

template <typename Impl>
class Motor: public Impl
{
public:
    unsigned char id;
    Segment segment;
    int _pin;
    int position;
    bool prepared;
    int prepared_position;
    long unsigned int prepared_time;
    bool reversed;  
    int _min;
    int _max;
    int offset;
    int motor_type;
    Motor(ConfigMotor & motor) : _pin(motor.pin), position(UNKNOWN), prepared(false), reversed(motor.reversed), _min(motor.min), _max(motor.max), offset(motor.offset) {}
    //Motor(int pin): _pin(pin), position(UNKNOWN), prepared(false), reversed(false), _min(-90), _max(90), offset(90), motor_type(0) {}
    Motor():_pin(UNKNOWN), position(UNKNOWN), reversed(false){}
    ~Motor(){}
    void init();
    void _write(double pos); 
    void write(double pos)
    {
    // входящие значение от _min до _max, чаще всего от -90 до 90
    
    
        if (pos < _min)
            pos = _min;
        else if (pos > _max)
            pos = _max;
    
                
        position = pos;
        // Serial.println("WRITING");
        
        /*
        if (reversed)
            _write(offset - pos);
        else    
            _write(offset + pos);
        */
        
        if (reversed)
            _write(135-(offset + pos));
        else    
            _write(135 + offset + pos);
        
        
        /*
        if (reversed)
            _write(270-(offset + pos));
        else
            _write(offset + pos);
        */
    }
};


#endif
