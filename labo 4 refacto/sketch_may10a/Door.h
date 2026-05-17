#pragma once
#include <Arduino.h>
#include <HCSR04.h>
#include "Motor.h"

enum Etat { CLOSE, OPENING, OPEN, CLOSING, EMERGENCY };

class Door {
private:
  Motor&  motor;
  HCSR04& sensor;
  Etat    state;
  float   distance;
  unsigned long lastDate;

  const float AREA1 = 0;
  const float AREA2 = 30;

  void checkDistance();

public:
  Door(Motor& m, HCSR04& s);
  void update();
  void openWithButton();
  void toggleEmergency();
  String getStatue() const;
};