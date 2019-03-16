#ifndef CLI_H
#define CLI_H
#define MAX_ARG_COUNT 5

extern bool whitespace(char c);

class CLI{
  
  enum State {
    ARG_SPACE, ARG
  } state;
public: 

  
  void append(char c);
  
  void start_arg();
  
  void drop();

  char cmd[48];
  char * args[MAX_ARG_COUNT];
  
  int currarg;
  int currcmd;
 
  
  bool feed(char c);

  void feed(const char * p);
  
  void feedln(char * p);
  
  CLI();
};
#endif

