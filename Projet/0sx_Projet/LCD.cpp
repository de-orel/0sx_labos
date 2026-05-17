#include "lcd.h"
#include <Arduino.h>

Lcd::Lcd()
  : lcd(0x27, 16, 2) {}

void Lcd::init() {
  lcd.begin();
  lcd.backlight();
}

// Labo 6 
void Lcd::printDiscount() {
  lcd.setCursor(0, 0);
  lcd.print("Mode: Rabais    ");
  lcd.setCursor(0, 1);
  lcd.print("                ");
}

void Lcd::printNormal() {
  lcd.setCursor(0, 0);
  lcd.print("Mode: Normal    ");
  lcd.setCursor(0, 1);
  lcd.print("                ");
}

void Lcd::printError() {
  lcd.setCursor(0, 0);
  lcd.print("Mode: Erreur    ");
  lcd.setCursor(0, 1);
  lcd.print("                ");
}

void Lcd::printClose() {
  lcd.setCursor(0, 0);
  lcd.print("Mode: Ferme     ");
  lcd.setCursor(0, 1);
  lcd.print("                ");
}

void Lcd::printEmergency() {
  lcd.setCursor(0, 0);
  lcd.print("!! URGENCE !!   ");
  lcd.setCursor(0, 1);
  lcd.print("Systeme arrete  ");
}

// Labo 4 
void Lcd::printDoorState(const String& state) {
  lcd.setCursor(0, 0);
  lcd.print("Porte:          ");
  lcd.setCursor(0, 1);
  lcd.print(state);
}

// Labo 5 
void Lcd::printConvoyeurInfo(const String& active, const String& position, int speed) {
  static unsigned long lastRefresh = 0;
  static int pageIndex = 0;

  if (millis() - lastRefresh >= 2500) {
    lastRefresh = millis();
    lcd.clear();

    switch (pageIndex) {
      case 0:
        lcd.setCursor(0, 0);
        lcd.print("Convoyeur actif:");
        lcd.setCursor(0, 1);
        lcd.print(active);
        break;
      case 1:
        lcd.setCursor(0, 0);
        lcd.print("Sens:           ");
        lcd.setCursor(0, 1);
        lcd.print(position);
        break;
      case 2:
        lcd.setCursor(0, 0);
        lcd.print("Vitesse:        ");
        lcd.setCursor(0, 1);
        lcd.print(active == "OUI" ? speed : 0);
        break;
    }
    pageIndex = (pageIndex + 1) % 3;
  }
}

void Lcd::clean() {
  lcd.clear();
}
