#pragma once
#include <Arduino.h>
#include <LCD_I2C.h>

class Lcd {
private:
  LCD_I2C lcd;

public:
  Lcd();
  void init();

  // Labo 6 - Affichage mode magasin
  void printDiscount();
  void printNormal();
  void printError();
  void printClose();
  void printEmergency();

  // Labo 4 - Etat de la porte
  void printDoorState(const String& state);

  // Labo 5 - Convoyeur (cyclage des infos)
  void printConvoyeurInfo(const String& active, const String& position, int speed);

  void clean();
};
