#include "head.h"
#include <Arduino.h>



Matrice::Matrice(int CLK_PIN, int DIN_PIN, int CS_PIN){
  clockPin = CLK_PIN;
  dataPin = DIN_PIN;
  chipSelectPin = CS_PIN;
  U8G2_MAX7219_8X8_F_4W_SW_SPI u8g2(U8G2_R0, CLK_PIN, DIN_PIN, CS_PIN, U8X8_PIN_NONE, U8X8_PIN_NONE);
}

void Matrice::begin(){
  u8g2.begin();
  u8g2.setContrast(5);
  u8g2.setFont(u8g2_font_4x6_tr);
  u8g2.clearBuffer();
  u8g2.sendBuffer();
}

void Matrice::drawAstring(String text){
  drawStr(x, y, text);
}