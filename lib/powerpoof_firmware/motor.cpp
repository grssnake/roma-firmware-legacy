#include "motor.h"
#include <HardwareSerial.h>


/*
double _map(double x, double in_min, double in_max, double out_min, double out_max)
{
  if (in_max == in_min)
     return out_max;             
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
*/

double _map(double x, double in_min, double in_max, double out_min, double out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
        

