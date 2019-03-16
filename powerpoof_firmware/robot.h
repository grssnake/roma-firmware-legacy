#ifndef ROBOT_H
#define ROBOT_H

#include "config.h"
#include "tools.h"
#include <motor.h>
#include <communication.h>
#include <CLI.h>
#include <binary_protocol_commander.h>


#define IMPLEMENT_ROBOT_MEMBER(impl, motor_type, comm_type) \
    BaseRobot<impl, motor_type, comm_type> 

#define ROBOT_BASE_IMPLEMENT(type) \
    template <> \
   type IMPLEMENT_ROBOT_MEMBER(ROBOT_TYPE, MOTOR_TYPE, COMM_TYPE)

#define CURRENT_INIT_STEP current_initialization_step
#define GO_TO_INIT_STEP(step) current_initialization_step=step
#define IS_CURRENT_INIT_STEP(step) current_initialization_step==step


#define INIT_STEP_START 0
#define INIT_STEP_CONFIG_READ 1
#define INIT_STEP_CONFIG_VALID 2
#define INIT_STEP_CONFIG_INVALID 3
#define INIT_STEP_MOTORS 4



#define INIT_STEP_FINISHED 100

#define INIT_FINISHED IS_CURRENT_INIT_STEP(INIT_STEP_FINISHED)



template <typename Impl, typename MotorType, typename CommunicatorType>
class BaseRobot:public Impl
{
public:
    
    #ifndef TEXT_PROTOCOL
    BinaryPacketCommander commander;
    #else
    CLI commander;
    #endif
    Config config;
    Motor<MotorType> motors[MOTOR_COUNT];
    Communicator<CommunicatorType> communicator;
    unsigned long int last_loop_mcs;
    unsigned long last_send_time=0;
    unsigned send_delay=4;
    unsigned current_initialization_step=INIT_STEP_START;
    bool is_any_motor_active=false;
    bool notify_on_motion_end=false;
    
    bool special_state=true;
    
    
    
    //BoleanProperties/////////////////
    bool initialized=false;
    bool config_is_valid=false;
    bool safe_mode=true;
    bool motors_initialized=false;
    
    
    //////////////////////////////////
    //CommunicatorType communicator;
    BaseRobot():Impl()
    {
    }
    //PlatformDependingMembers//////////////
    void read_config_from_storage();
    bool write_config_to_storage();
    void init_communicator(bool wired);
    unsigned long _millis();
    void init_sound();
    void play_sound(const char * string);
    float get_sensor_data(int i){ return 0;};
    float get_us_sensor_data(int i) { return 0;};
    void stop_sound();
    
    void update_sensors();
    void start_iteration();
    void finish_iteration();
    void acc_values(float * values);
    void compass_values(float * values);
    void gyro_values(float * values);
    void restart();
    bool validate_config();
    bool break_storage_config();
    unsigned long get_config_crc_from_storage();
    unsigned get_available_ram();
    //CommandsHandling///////////////////////////////
    void perform_knock();
    void perform_move_done();
    
    ////////////////////////////////////////////////////
    void _perform_outcome_stream();
    void listen_for_command();
#ifdef TEXT_PROTOCOL
    void perform_text_command(char ** args);
    void reply_about_safe_mode(const char * header);
#else
    void perform_command(unsigned char * data, unsigned long length);
#endif
    void init_safe_elements()
    {
        read_config_from_storage();
        initialized=true;
    }
    void init_unsafe_elements()
    {
        
    }
    void apply_config()
    {
        
    }
    void reply_on_knock()
    {
        //cprintln("knock who's there");
    }
    void handle_special_state()
    {

        if (IS_CURRENT_INIT_STEP(INIT_STEP_START))
        {
            GO_TO_INIT_STEP(INIT_STEP_CONFIG_READ);
        }
        else if (IS_CURRENT_INIT_STEP(INIT_STEP_CONFIG_READ))
        {
            if (validate_config())
            {   
                GO_TO_INIT_STEP(INIT_STEP_CONFIG_VALID);
            }
            else
            {
                GO_TO_INIT_STEP(INIT_STEP_CONFIG_INVALID);
            }
        }
        else if (IS_CURRENT_INIT_STEP(INIT_STEP_CONFIG_INVALID))
        {
        }
        else if (IS_CURRENT_INIT_STEP(INIT_STEP_CONFIG_VALID))
        {
            GO_TO_INIT_STEP(INIT_STEP_MOTORS);
        }
        else if (IS_CURRENT_INIT_STEP(INIT_STEP_MOTORS))
        {
            init_motors();
            GO_TO_INIT_STEP(INIT_STEP_FINISHED);
        }
        else if (IS_CURRENT_INIT_STEP(INIT_STEP_FINISHED))
        {
            special_state=false;
        }
    }
    void init_motors()
    {
        //this->init_motors_impl();
        for (int i=0; i <MOTOR_COUNT; i++)
        {   
            //unsigned char motor_id=config.motors[i].id;
            motors[i]=Motor<MotorType>(config.motors[i]);
            motors[i].init();
        }
    } 
    void to_zero_position_all()
    {
        for (int i=0; i <MOTOR_COUNT; i++)
        {
            start_move(motors[i], 0, 1, 0);
        }
    }
    void to_zero_position(Motor<MotorType> & motor)
    {
       start_move(motor, 0, 1, 0);
    }
    void refresh_offset(Motor<MotorType> & motor)
    {
        start_move(motor, motor.position, 1, 0);
    }
    void set_safe_mode()
    {
        safe_mode=true;
    }
    void set_full_mode()
    {
        safe_mode=false;
    }
     void disable_all_segments()
    {
        for (int i=0; i<MOTOR_COUNT; i++)
        {
            motors[i].segment.active = false;
        }
    }


    void jump_to_start()
    {
        for (int i=0; i<MOTOR_COUNT; i++)
        {
            motors[i].segment.active = false;
            motors[i].write(start[i]);
        }
    }
    int start_move(Motor<MotorType> & motor, int to, int time_ms, bool notify=false)
    {
        notify_on_motion_end=notify;
        motor.segment.active=true;    
        motor.segment.begin = motor.position;
        motor.segment.end = to;
        motor.segment.start = _millis();
        motor.segment.finish = motor.segment.start + time_ms;
    }
    


    int prepare_move(Motor<MotorType> & motor, int to, int time_ms)
    {    
        motor.prepared = true;
        motor.prepared_position = to;
        motor.prepared_time = time_ms;  
    }

    void perform_outcome_stream()
    {
        if (_millis()-last_send_time > send_delay)
        {
            
            _perform_outcome_stream();
            last_send_time=_millis();   
        }
    }
    
    void update_motors()
    {
        bool is_any_active=false;
        //cprintln(is_any_active);
        unsigned long int current = _millis();
        for (int i = 0; i < MOTOR_COUNT; i++)
        {

            Motor<MotorType> & motor = motors[i];
            Segment & segment = motor.segment;
            if (segment.active)
            {
                is_any_active=true;
                if (segment.finish < current)
                {            
                    motor.write(segment.end);        
                    segment.active = false;
                    //print("move done");
                    //println(motor._pin);
                }
                else
                {   
                    double pos = _map((double)current, (double)segment.start, (double)segment.finish, (double) segment.begin, (double) segment.end);
                    motor.write(pos);
                }
            }
        }    
        if (!is_any_active && is_any_motor_active)
        {
            if (notify_on_motion_end)
            {
                perform_move_done();
                notify_on_motion_end=false;
            }
        }
        is_any_motor_active=is_any_active;
    }
};

////////////////////////////////////////////////////////////////////////////


#ifdef TEXT_PROTOCOL
#include "robot_text_proto_impl.tpp"
#else
#include "robot_binary_proto_impl.tpp"
#endif


#endif
