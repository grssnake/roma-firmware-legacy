#ifndef CONFIG_H
#define CONFIG_H
// 1 classic mode with 11 and 17 swapped
// 2 new mode, straight
// 3 sensorshield mode, direct arduino servo

#define ROMA_CLASSIC 1
#define ROMA_DIRECT 2
#define ROMA_SENSORSHIELD 3 
#define ROMA ROMA_CLASSIC


#define MOTOR_COUNT 19
#define MOTOR_TYPE_180 1
#define MOTOR_TYPE_270 2 



#pragma pack(push,1)
struct Config2
{
    int start[MOTOR_COUNT]={0};
    bool reversed[MOTOR_COUNT]={false};
    int motor_offset[MOTOR_COUNT]={0};
    int motor_min[MOTOR_COUNT]={0};
    int motor_max[MOTOR_COUNT]={0};
    int motor_types[MOTOR_COUNT]={0};
    char motor_pin_map[MOTOR_COUNT]={0};
    char reset_pin;
    bool bluetooth_connection;
    
};


struct ConfigMotor
{ 
    unsigned char id=0;
    int start=0;
    bool reversed=false;
    int offset=0;
    int min=0;
    int max=0;
    unsigned char pin=0;
    
    
};
struct Config
{
    ConfigMotor motors[MOTOR_COUNT];
    unsigned char reset_pin;
};
#pragma pack(pop)

int find_config_motor(Config & config,unsigned id);



extern int start[MOTOR_COUNT];
extern bool reversed[MOTOR_COUNT];
extern int motor_offset[MOTOR_COUNT];
extern int motor_min[MOTOR_COUNT];
extern int motor_max[MOTOR_COUNT];
extern int motor_types[MOTOR_COUNT];
extern char motor_pin_map[MOTOR_COUNT];
extern char reset_pin;
extern bool bluetooth_connection;




    
#endif
