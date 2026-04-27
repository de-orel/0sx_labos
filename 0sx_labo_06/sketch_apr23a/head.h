#pragma once
#include <Arduino.h>


class Matrice{
private:
  int clockPin;
  int dataPin;
  int chipSelectPin;

public:
  Matrice(int CLK_PIN, int DIN_PIN, int CS_PIN);
  void begin();
  void drawAstring(String text);
};

class Remote{

private:
  int receive_pin;

public:
  Remote(int receive_pin);
  void read();
  void discount();
  void normal();
  void error();
  void close();
  void emergency();
};

class Lcd{

private:
  
public:

};
