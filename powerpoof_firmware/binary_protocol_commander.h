#ifndef BINARY_PROTOCOL_COMMANDER_H
#define BINARY_PROTOCOL_COMMANDER_H
#include "tools.h"


class BinaryPacketCommander
{
public:
    BinaryPacketCommander()
    {
    }
    unsigned char buffer[128];
    unsigned char cur_pos=0;
    
    int feed(unsigned char b)
    {
        buffer[cur_pos]=b;
        cur_pos++;
        if (buffer[0]==cur_pos)
        {
            uint32_t crc=*reinterpret_cast<uint32_t*>(buffer+cur_pos-4);
            if (get_mem_crc(buffer, cur_pos-4)==crc)
                return 1;
            else
                return -1;
           
        }
        return 0;
    }
    void drop()
    {
        cur_pos=0;
    }
};

#endif
