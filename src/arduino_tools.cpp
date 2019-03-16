#include "arduino_tools.h"
#include <avr/pgmspace.h>
#include <HardwareSerial.h>

bool EEPROM_put(unsigned address, unsigned char b)
{
    if (address<EEPROM_SIZE)
    {
        if (b!=EEPROM.read(address))
        {
            EEPROM.write(address, b);
            return true;
        }
    }
    return false;
}

bool EEPROM_update(unsigned address, void * data, unsigned size)
{
    bool changed=false; 
    
    if (address+size<EEPROM_SIZE)
    {
        unsigned char * tmp_data=static_cast<unsigned char*>(data);
        for (unsigned i=address;i<address+size;i++,tmp_data++)
        {
            if (EEPROM_put(i, *tmp_data))
            {
                changed=true;
            }
        }
    }
    return changed;
    
}

void EEPROM_read(unsigned address, void * data, unsigned size)
{
    if (address+size<EEPROM_SIZE)
    {
        unsigned char * tmp_data=static_cast<unsigned char*>(data);
        for (unsigned i=address;i<address+size;i++,tmp_data++)
        {
            *tmp_data=EEPROM.read(i);
        }
    }
}
unsigned long EEPROM_crc(unsigned address, unsigned size)
{

    unsigned long crc = ~0L;

    for (unsigned index = address ; index < address+size  ; index++) 
    {
        crc = crc_table[(crc ^ (unsigned char)EEPROM.read(index)) & 0x0f] ^ (crc >> 4);
        crc = crc_table[(crc ^ ((unsigned char)EEPROM.read(index) >> 4)) & 0x0f] ^ (crc >> 4);
        crc = ~crc;
    }
    return crc;
}



void read_from_flash(const char * str,char * dst,unsigned size,bool zero_terminate)
{
    int len = strlen_P(str);
    for (int k = 0; k < len; k++, dst++)
    {
        *dst=pgm_read_byte_near(str + k);
    }
    if (zero_terminate)
        *(dst)=0;
}

void free_serial_income_buffer(HardwareSerial & serial)
{
    while(serial.available())
        serial.read();
}


