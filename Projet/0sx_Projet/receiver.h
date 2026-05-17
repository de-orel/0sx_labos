#pragma once
#include <Arduino.h>

class Receiver {
private:
  int receive_pin;

public:
  Receiver(int pin);
  void begin();
  unsigned long getCode();
};
