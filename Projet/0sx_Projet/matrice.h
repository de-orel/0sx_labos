#pragma once
#include <Arduino.h>
#include <U8g2lib.h>

class Matrice {
private:
  int clockPin;
  int dataPin;
  int chipSelectPin;
  U8G2_MAX7219_8X8_F_4W_SW_SPI u8g2;

public:
  Matrice(int CLK_PIN, int DIN_PIN, int CS_PIN);
  void begin();
  void scrollString(String text);
  void drawSmiley();
  void toggleOnOff();
  void shutDown();
  void emergency();
  void clean();
};
