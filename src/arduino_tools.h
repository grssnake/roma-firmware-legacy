#ifndef ARDUINO_TOOLS_H
#define ARDUINO_TOOLS_H
#include <tools.h>
#include <EEPROM.h>
#include <HardwareSerial.h>



#define EEPROM_SIZE 1024

#define ZERO_OBJECT(object) zero_mem(&object, sizeof(object));

#define EEPROM_READ(address,object) EEPROM_read(address,&object, sizeof(object));

#define EEPROM_UPDATE(address,object) EEPROM_update(address,&object, sizeof(object))

bool EEPROM_put(unsigned address, unsigned char b);

bool EEPROM_update(unsigned int address, void * data, unsigned size);

void EEPROM_read(unsigned int address, void * data, unsigned size);

unsigned long EEPROM_crc(unsigned int address, unsigned size);


void read_from_flash(const char * str,char * dst, unsigned size, bool zero_terminate=false);

void free_serial_income_buffer(HardwareSerial & serial);

#endif


