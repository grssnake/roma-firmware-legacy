#ifndef ROBOT_TEXT_PROTO_IMPL_TPP
#define ROBOT_TEXT_PROTO_IMPL_TPP


#define cprint communicator.print
#define cprintln communicator.println
#define cprintws communicator.printws
#define cprintlnws communicator.printlnws
#define cprint_header(header, msg) cprint(header); cprintws(msg)
#define cprintln_header(header, msg) cprint(header); cprintlnws(msg)
#define cprint_headerws(header, msg) cprintws(header); cprintws(msg)
#define cprintln_headerws(header, msg) cprintws(header); cprintlnws(msg)

#define DECLARE_STRING(name) const char name[]= #name
#define DECLARE_NSTRING(text, vname) const char vname[]= #text

#define CMD_IS(x) (!strcmp(args[0], x))
#define CMD args[0]
#define INT_ARG(i) atoi(args[i+1])
#define STRING_ARG(i) args[i+1]
#define STRING_ARG_IS(i,x)  !strcmp(args[i+1], x)
#define SEND_CMD_REPLY cprint(CMD);
DECLARE_STRING(ok);
DECLARE_STRING(fail);
DECLARE_STRING(move);
DECLARE_NSTRING(motor, motor_str);
DECLARE_STRING(zero_offset);
DECLARE_STRING(offset);

#define IMPLEMENT_TEMPLATE_MEMBER(type) \
    template <typename Impl, typename MotorType, typename CommunicatorType> \
    type BaseRobot<Impl,MotorType,CommunicatorType>
    
   



template <typename Impl, typename MotorType, typename CommunicatorType>
void BaseRobot<Impl,MotorType,CommunicatorType> ::listen_for_command()
{
    


    int c=communicator.read();
    if (c!=-1)
    {
        if (commander.feed(c))
        {
            perform_text_command(commander.args);
            commander.drop();
        }
    }
    //Serial.println(commander.cmd);

}

template <typename Impl, typename MotorType, typename CommunicatorType>
void BaseRobot<Impl,MotorType,CommunicatorType>::_perform_outcome_stream()
{
    
} 

template <typename Impl, typename MotorType, typename CommunicatorType>
void BaseRobot<Impl,MotorType,CommunicatorType> ::perform_text_command(char ** args)
{
    if (INIT_FINISHED && CMD_IS("start"))
    {
       disable_all_segments();
       jump_to_start();  
    }
    ////////////////////////////////
    else if (CMD_IS("hello"))
    {   
        cprint(CMD);
        cprintln(" hi"); 
    }
    //////////////////////////////
    else if (CMD_IS("knock"))
    {   
        cprintln("knock who's there");
        //reply_on_knock();
    }  
    //////////////////////////////
    else if (INIT_FINISHED && CMD_IS("go")) 
    {   
        if (safe_mode)
        {
            reply_about_safe_mode(move);
            return;
        }
        Motor<MotorType> & motor = motors[atoi(args[1])];
        int value = atoi(args[2]);
        int time = atoi(args[3]);
        start_move(motor, value, time,atoi(args[4]));
    }
    //////////////////////////////
    else if (INIT_FINISHED &&  CMD_IS("move")) 
    {       
        if (safe_mode)
            return;
        Motor<MotorType> & motor = motors[atoi(args[1])];
        int value = atoi(args[2]);
        int time = atoi(args[3]);
        prepare_move(motor, value, time);     


    }
    //////////////////////////////
    else if (INIT_FINISHED && CMD_IS("commit")) 
    {      
        if (safe_mode)
        {
            reply_about_safe_mode(move);
            return;
        } 
        for (int i=0; i<MOTOR_COUNT; i++)
        {
            Motor<MotorType> & motor = motors[i];
            if (motor.prepared)
            {
                start_move(motor, motor.prepared_position, motor.prepared_time, atoi(args[1]));
                motor.prepared = false;
            }
        }
 
     }
     //////////////////////////////
     else if (CMD_IS("init_state"))
     {
        cprint(CMD);
        if (INIT_FINISHED)
        {
            cprintln(" finished");
        }
        else
        {
            cprintlnws(CURRENT_INIT_STEP);
        }

     }
     //////////////////////////////
     else if (CMD_IS("mode"))
     {
        DECLARE_STRING(safe);
        DECLARE_STRING(full);
        
        cprint(CMD);
        if (STRING_ARG_IS(0,"set"))
        {
            if (STRING_ARG_IS(1,safe))
                set_safe_mode();
            else if (STRING_ARG_IS(1,full))
                set_full_mode();
            cprintlnws(ok); 
        }
        else if (STRING_ARG_IS(0,"get"))
        {
            if (safe_mode)
                cprintlnws(safe);
            else
                cprintlnws(full);
        }
     }
     //////////////////////////////
     else if (INIT_FINISHED && CMD_IS(zero_offset))
     {
        if (safe_mode)
        {
            reply_about_safe_mode(motor_str);
            return;
        }
        if (STRING_ARG_IS(0,"all"))
        {
            to_zero_position_all();
            cprintln_header(zero_offset, ok);
        }
        else
        {
            Motor<MotorType> & motor = motors[INT_ARG(0)];
            refresh_offset(motor);
            cprintln_header(zero_offset, ok);
        }
        
     }
     //////////////////////////////
     else if(CMD_IS("motor"))
     {
        if (INIT_FINISHED && STRING_ARG_IS(0,"set"))
        {
            if (safe_mode)
            {
                reply_about_safe_mode(motor_str);
                return;
            }
            if (STRING_ARG_IS(1, "offset"))
            {
                Motor<MotorType> & motor = motors[INT_ARG(2)];
                motor.offset=INT_ARG(3);
                cprintln_header(motor_str, ok);
            }
        }
        else if (INIT_FINISHED && STRING_ARG_IS(0,"get"))
        {
            Motor<MotorType> & motor = motors[INT_ARG(1)];
            cprintws(INT_ARG(1));
            cprintws(motor.position);
            cprintws(motor.offset);
            cprintlnws(ok);  
        }
     }
     //////////////////////////////
     else if (CMD_IS("config"))
     {
        DECLARE_STRING(motor);
        DECLARE_STRING(start);
        DECLARE_STRING(reversed);
        DECLARE_STRING(offset);
        DECLARE_STRING(min);
        DECLARE_STRING(max);
        DECLARE_STRING(pin);
        cprint(CMD);
        if (STRING_ARG_IS(0,"get"))
        {
            if (STRING_ARG_IS(1,motor))
            {
                //Motor<MotorType> & motor=motors[INT_ARG(2)];
                ConfigMotor & motor=config.motors[INT_ARG(2)];
                cprintws(INT_ARG(2));
                cprintws(motor.start);
                cprintws((bool)motor.reversed);
                cprintws(motor.offset);
                cprintws(motor.min);
                cprintws(motor.max);
                cprintws(motor.pin); 
                cprintlnws(ok);           
            }
        }
        else if (STRING_ARG_IS(0,"set"))
        {
            if (STRING_ARG_IS(1,motor))
            {
                //int id=find_config_motor(config, INT_ARG(2));
                ConfigMotor & motor=config.motors[INT_ARG(2)];
                if (STRING_ARG_IS(3,start))
                    motor.start=INT_ARG(4);
                if (STRING_ARG_IS(3,reversed))
                    motor.reversed=INT_ARG(4);
                if (STRING_ARG_IS(3,offset))
                    motor.offset=INT_ARG(4);
                if (STRING_ARG_IS(3,max))
                    motor.max=INT_ARG(4);
                if (STRING_ARG_IS(3,min))
                    motor.min=INT_ARG(4);
                if (STRING_ARG_IS(3,pin))
                    motor.pin=INT_ARG(4);
                
            }
            cprintlnws(ok);
        }
        else if (STRING_ARG_IS(0,"from_motors"))
        {
            if (STRING_ARG_IS(1,offset))
            {
                for (int i=0;i<MOTOR_COUNT;i++)
                {
                    config.motors[i].offset=motors[i].offset;
                }
                cprintlnws(ok);
            }
        }
        else if (STRING_ARG_IS(0,"fix"))
        {
            if (IS_CURRENT_INIT_STEP(INIT_STEP_CONFIG_INVALID))
            {
                //command_save_config(false);
                
                if (validate_config())
                {   
                    cprintlnws("fixed");
                    GO_TO_INIT_STEP(INIT_STEP_CONFIG_VALID);
                    return;
                }
                cprintlnws("not fixed");
                return;
                    
            }
            cprintlnws("nothing to fix");
            
        }
        
        else if (STRING_ARG_IS(0,"save"))
        {   
           
            char * reply;
            if (write_config_to_storage())
                reply="changed";
            else
                reply="unchanged";
                
            cprintlnws(reply);
                
        }
        else if (STRING_ARG_IS(0,"crc"))
        {
            cprintlnws(OBJECT_CRC(config));
        }
        else if (STRING_ARG_IS(0,"break"))
        {
            if (break_storage_config())
                cprintlnws(ok);
            else
                cprintlnws(fail);
        }
        
     }
     //////////////////////////////
}

template <typename Impl, typename MotorType, typename CommunicatorType>
void BaseRobot<Impl,MotorType,CommunicatorType> ::reply_about_safe_mode(const char * header)
{
    cprint(header);
    cprintlnws("safe");
}


template <typename Impl, typename MotorType, typename CommunicatorType>
void BaseRobot<Impl,MotorType,CommunicatorType> ::perform_knock()
{
    cprintln("knock who's there");
}
template <typename Impl, typename MotorType, typename CommunicatorType>
void BaseRobot<Impl,MotorType,CommunicatorType> ::perform_move_done()
{
    cprintln("move all_ok");
}

#endif
