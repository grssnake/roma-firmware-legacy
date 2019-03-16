#ifndef ROBOT_BINARY_PROTO_IMPL_TPP
#define ROBOT_BINARY_PROTO_IMPL_TPP

template <typename Impl, typename MotorType, typename CommunicatorType>
void BaseRobot<Impl,MotorType,CommunicatorType> ::listen_for_command()
{
    while(communicator.available())
    {
        int r=commander.feed(communicator.read());    
        if (r==1)
        {
            perform_command(commander.buffer, commander.cur_pos);
        }
        else if (r==-1)
        {
        }
        else
            continue;
    }
}

template <typename Impl, typename MotorType, typename CommunicatorType>
void BaseRobot<Impl,MotorType,CommunicatorType> ::_perform_outcome_stream()
{
    
} 

#endif
