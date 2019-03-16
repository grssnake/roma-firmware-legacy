#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H
//#error("COMM")
#include "config.h"
#include <HardwareSerial.h>
#include <CLI.h>
#ifdef COMMANDS_TEXT

#endif




template <typename Impl>
class Communicator:public Impl
{     
public:
    Communicator()
    {
        
    }
        
    void init();
    unsigned available();
    int read();
    void clear()
    {
        while(available())
            read();
    }
    
#ifndef TEXT_PROTOCOL
    void write(unsigned char b);
    void write(unsigned char * data, unsigned  long length);
#else
    void println();
    template <typename T>
    void print(T n);
    template <typename T>
    void println(T n);
    template <typename T>
    void printws(T n) {print(" "); print(n);}
    template <typename T>
    void printlnws(T n) {print(" "); println(n);}
#endif
   
    
};
#endif
