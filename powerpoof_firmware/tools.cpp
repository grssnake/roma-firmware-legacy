
#include "tools.h"

void zero_mem(void * data, unsigned size)
{
    for (char * i=static_cast<char*>(data);i<data+size;i++)
    {
        *i=0;
    }
}

uint32_t get_mem_crc(void * data, unsigned size)
{
    
    uint32_t crc = ~0L;
    for (unsigned char * i=static_cast<unsigned char*>(data);i<data+size;i++)
    {
        crc = crc_table[(crc ^ *i) & 0x0f] ^ (crc >> 4);
        crc = crc_table[(crc ^ (*i >> 4)) & 0x0f] ^ (crc >> 4);
        crc = ~crc;
    }
    return crc;
}

