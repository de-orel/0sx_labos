#include "matrice.h"
#include <Arduino.h>

Matrice::Matrice(int CLK_PIN, int DIN_PIN, int CS_PIN) : u8g2(U8G2_R0, CLK_PIN, DIN_PIN, CS_PIN, U8X8_PIN_NONE, U8X8_PIN_NONE)
{
  clockPin = CLK_PIN;
  dataPin = DIN_PIN;
  chipSelectPin = CS_PIN;
}

void Matrice::begin() {
  u8g2.begin();
  u8g2.setContrast(5);
  u8g2.setFont(u8g2_font_4x6_tr);
  u8g2.clearBuffer();
  u8g2.sendBuffer();
}

void Matrice::clean(){
  u8g2.clearBuffer();
  u8g2.sendBuffer(); 
}

void Matrice::emergency() {
  const uint8_t bitmapCross[8] = {
    B10000001,
    B01000010,
    B00100100,
    B00011000,
    B00011000,
    B00100100,
    B01000010,
    B10000001
  };

  u8g2.clearBuffer();
  u8g2.drawBitmap(0, 0, 1, 8, bitmapCross);
  u8g2.sendBuffer();
}

void Matrice::toggleOnOff() {
  static unsigned long lastUpdate = 0;
  static bool isOn = false;
  // 1. TOUT ALLUMER
  if(millis() - lastUpdate >= 200 && isOn == false){
    lastUpdate = millis();
    isOn = true;

    u8g2.clearBuffer();          
    u8g2.drawBox(0, 0, 8, 8);    
    u8g2.sendBuffer();
  } 
  else if(millis() - lastUpdate >= 200 && isOn == true){   // 2. TOUT ÉTEINDRE
    lastUpdate = millis();
    isOn = false;

    u8g2.clearBuffer();          
    u8g2.sendBuffer(); 
  }
         
}

void Matrice::shutDown(){
  u8g2.clearBuffer();          
  u8g2.sendBuffer();   
}

void Matrice::drawSmiley(){

  const uint8_t bitmapSmiley[8] = {
    B00111100,
    B01000010,
    B10100101,
    B10000001,
    B10100101,
    B10011001,
    B01000010,
    B00111100
  };

  u8g2.clearBuffer();
  u8g2.drawBitmap(0, 0, 1, 8, bitmapSmiley);
  u8g2.sendBuffer();
}

void Matrice::scrollString(String text) {

  static long lastScroll = 0;                                
  
  if(millis() - lastScroll >= 70){  // vitesse de défilement
    lastScroll = millis();
    static int x_pos = 8;        // Position horizontale du texte  // On commence juste au bord droit de la matrice (8 pixels)

    const char *message = text.c_str();
    int largeur_txt = u8g2.getStrWidth(message);  // Largeur totale du texte en pixels    // Calcule automatiquement la largeur du texte
    
    u8g2.clearBuffer();
    u8g2.drawStr(x_pos, 7, message);  // On dessine le texte à la position X
    u8g2.sendBuffer();

    x_pos--;  // On déplace vers la gauche

    // Si le texte est complètement sorti par la gauche, on recommence à droite
    if (x_pos < -largeur_txt) {
      x_pos = 8;
    }
  }
}