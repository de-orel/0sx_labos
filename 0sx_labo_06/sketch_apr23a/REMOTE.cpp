#include "head.h"
#include <Arduino.h>

Remote::Remote(int receive_pin){
  this->receive_pin = receive_pin;
}