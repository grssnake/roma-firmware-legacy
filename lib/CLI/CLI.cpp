#include <string.h>
#include "CLI.h"
#include <HardwareSerial.h>

bool whitespace(char c){
  return c == ' ';
}

  void CLI::append(char c){
     cmd[currcmd++] = c;
     cmd[currcmd] = 0;
  }
  
  void CLI::start_arg(){
    currcmd++;    
    args[currarg++] = cmd + currcmd;
  }
  
  void CLI::drop(){
    cmd[0] = 0;
    currcmd = -1;
    currarg = 0;
    state  = ARG_SPACE;    
  }

  

  
    bool  CLI::feed(char c)
    {
        if (c==0 || c =='\n' || c == '\r')
        {
            return true;
           //drop();
        }
        else if (c == '\b')
          drop();
        else   
            switch (state)
            {
                case ARG_SPACE:
                if (!whitespace(c))
                {
                    state = ARG; //fallthrough
                    start_arg();
                }
                case ARG:
                if (whitespace(c))
                  state = ARG_SPACE;
                else 
                  append(c);
            }
        return false;
    } 

  void CLI::feed(const char * p){
    while (*p){
        feed(*(p++));
        }
  }
  
  void CLI::feedln(char * p){
    feed(p);
    feed('\n');
  }  

  CLI::CLI(){
    drop();
  }
