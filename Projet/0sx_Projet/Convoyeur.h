#pragma once
#include <Arduino.h>
#include "MoteurDC.h"

enum EtatConvoyeur { CONVOYEUR_REST,
                     CONVOYEUR_MOVE,
                     CONVOYEUR_EMERGENCY };

class Convoyeur {
private:
  MoteurDC& motor;

  EtatConvoyeur state;
  int xValue;
  int yValue;
  unsigned long lastSpeedUpdate;

  const int DEADZONE = 50;
  const int PIN_JOY_X;
  const int PIN_JOY_Y;
  const int PIN_LED_MOVE;
  const int PIN_LED_EMERGENCY;

  void checkJoystick();
  void controlMovement();
  void controlSpeed();

public:
  Convoyeur(MoteurDC& m, int joyX, int joyY, int ledMove, int ledEmergency);
  void begin();
  void update();
  void toggleEmergency();

  EtatConvoyeur getState() const;
  String getActiveLabel() const;
  String getPositionLabel() const;  // Pour LCD : AVANT / ARRIERE / STATIONNAIRE
  String getDirectionMqtt() const;  // Pour MQTT : gauche / droite / stop
  int getSpeed() const;
};
