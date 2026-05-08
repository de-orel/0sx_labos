#include "lcd.h"
#include <Arduino.h>

Lcd::Lcd() : lcd(0x27, 16, 2)
{}
void Lcd::init(){
  lcd.begin();
  lcd.backlight();
}
void Lcd::printDiscount(){
  lcd.setCursor(0, 0);
  lcd.print("Mode: Rabais");
}
void Lcd::printNormal(){
  lcd.setCursor(0, 0);
  lcd.print("Mode: Normal");
}
void Lcd::printError(){
  lcd.setCursor(0, 0);
  lcd.print("Mode: Erreur");
}
void Lcd::printClose(){
  lcd.setCursor(0, 0);
  lcd.print("Mode: Ferme");
}
void Lcd::printEmergency(){
  lcd.setCursor(0, 0);
  lcd.print("Mode : !! URGENCE !!");
}
void Lcd::clean(){
  lcd.clear();
}


