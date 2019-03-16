#include "config.h"
    
int find_config_motor(Config & config,unsigned id)
{
    for (int i=0;i<MOTOR_COUNT; i++)
    {
        if (config.motors[i].id==id)
            return i;
    }
    return -1;
}    
    
    char reset_pin=4;
    bool bluetooth_connection=false;

    int start[MOTOR_COUNT] = {
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      //10      11     12      13
      0,        0,     0,      0,
      //14      15     16      17
      0,       0,     0,    0,
      
      0  
    };

    bool reversed[MOTOR_COUNT] = {
      //0      1     2      3      4
      false, true, false, true, true,
      //5      6     7      8      9
      true, false, true, false, false,
      //10      11     12      13
      false, true, false, true,
      //14      15     16      17
      true, false, true, false,
      // 18
      false
    };

    int motor_offset[MOTOR_COUNT] = {
    
    135, 135, 135, 135, 135,
    
    135, 135, 135, 135, 135,
    
    135, 135, 135, 135, 135,
    
    135, 135, 135, 135
    
    };

    int motor_min[MOTOR_COUNT] = {
    -135, -135, -135, -135, -135,
    -135, -135, -135, -135, -135,
    -135, -135, -135, -135,
    -135, -135, -135, -135,
    -135
    };

    int motor_max[MOTOR_COUNT] = {
    135, 135, 135, 135, 135,
    135, 135, 135, 135, 135,
    135, 135, 135, 135,
    135, 135, 135, 135,
    135
    };

    int motor_types[MOTOR_COUNT] = {
      MOTOR_TYPE_270, MOTOR_TYPE_270, MOTOR_TYPE_270, MOTOR_TYPE_270, MOTOR_TYPE_270,
      MOTOR_TYPE_270, MOTOR_TYPE_270, MOTOR_TYPE_270, MOTOR_TYPE_270, MOTOR_TYPE_270,
      MOTOR_TYPE_270, MOTOR_TYPE_270, MOTOR_TYPE_270,MOTOR_TYPE_270,
      MOTOR_TYPE_270, MOTOR_TYPE_270, MOTOR_TYPE_270,MOTOR_TYPE_270,
      MOTOR_TYPE_270
    };
     char motor_pin_map[19] = {
        0,  1,  2,  3,  4,
        5,  6,  7,  8,  9,
        10, 11, 12, 13, 14,
        15, 16, 17, 18
        };
