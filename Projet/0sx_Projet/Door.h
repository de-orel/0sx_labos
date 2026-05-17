#pragma once
#include <Arduino.h>
#include <HCSR04.h>
#include "Motor.h"

enum EtatPorte { PORTE_CLOSE, PORTE_OPENING, PORTE_OPEN, PORTE_CLOSING, PORTE_EMERGENCY };

class Door {
private:
  Motor& motor;
  HCSR04& sensor;
  EtatPorte state;
  float distance;
  unsigned long lastDate;
  int clientCount;

  const float AREA1 = 0;
  const float AREA2 = 30;

  void checkDistance();

public:
  Door(Motor& m, HCSR04& s);
  void update();
  void openWithButton();
  void toggleEmergency();
  String getStatue() const;
  String getPorteLabel() const;
  int getClientCount() const;
};
