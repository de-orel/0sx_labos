#pragma once
#include <Arduino.h>
#include <LCD_I2C.h>

class Lcd{
private:
  LCD_I2C lcd;
public:
  Lcd();
  void init();
  void printDiscount();
  void printNormal();
  void printError();
  void printClose();
  void printEmergency();
  void clean();
};