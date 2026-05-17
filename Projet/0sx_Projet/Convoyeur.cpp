#include "Convoyeur.h"

Convoyeur::Convoyeur(MoteurDC& m, int joyX, int joyY, int ledMove, int ledEmergency)
  : motor(m),
    state(CONVOYEUR_REST),
    xValue(0), yValue(0), lastSpeedUpdate(0),
    PIN_JOY_X(joyX), PIN_JOY_Y(joyY),
    PIN_LED_MOVE(ledMove), PIN_LED_EMERGENCY(ledEmergency) {}

void Convoyeur::begin() {
  pinMode(PIN_LED_MOVE, OUTPUT);
  pinMode(PIN_LED_EMERGENCY, OUTPUT);
}

void Convoyeur::update() {
  switch (state) {
    case CONVOYEUR_REST:
      motor.shutDown();
      digitalWrite(PIN_LED_MOVE, LOW);
      digitalWrite(PIN_LED_EMERGENCY, LOW);
      checkJoystick();
      break;

    case CONVOYEUR_MOVE:
      digitalWrite(PIN_LED_MOVE, HIGH);
      digitalWrite(PIN_LED_EMERGENCY, LOW);
      controlMovement();
      controlSpeed();
      checkJoystick();
      break;

    case CONVOYEUR_EMERGENCY:
      motor.shutDown();
      digitalWrite(PIN_LED_MOVE, LOW);
      digitalWrite(PIN_LED_EMERGENCY, HIGH);
      break;
  }
}

void Convoyeur::toggleEmergency() {
  if (state == CONVOYEUR_EMERGENCY) {
    state = CONVOYEUR_REST;
  } else {
    state = CONVOYEUR_EMERGENCY;
  }
}

void Convoyeur::checkJoystick() {
  yValue = analogRead(PIN_JOY_Y);
  if (abs(yValue - 512) > DEADZONE) {
    state = CONVOYEUR_MOVE;
  } else {
    state = CONVOYEUR_REST;
  }
}

void Convoyeur::controlMovement() {
  if (yValue > 512 + DEADZONE) {
    motor.moveForward();
  } else if (yValue < 512 - DEADZONE) {
    motor.moveBackward();
  }
}

void Convoyeur::controlSpeed() {
  xValue = analogRead(PIN_JOY_X);
  if (millis() - lastSpeedUpdate > 50) {
    if (xValue > 512 + DEADZONE) {
      motor.increaseSpeed();
      lastSpeedUpdate = millis();
    } else if (xValue < 512 - DEADZONE) {
      motor.decreaseSpeed();
      lastSpeedUpdate = millis();
    }
  }
}

EtatConvoyeur Convoyeur::getState() const {
  return state;
}

String Convoyeur::getActiveLabel() const {
  return (state == CONVOYEUR_MOVE) ? "OUI" : "NON";
}

String Convoyeur::getPositionLabel() const {
  if (state != CONVOYEUR_MOVE) return "STATIONNAIRE";
  if (yValue > 512 + DEADZONE) return "AVANT";
  if (yValue < 512 - DEADZONE) return "ARRIERE";
  return "STATIONNAIRE";
}

int Convoyeur::getSpeed() const {
  return motor.getSpeed();
}

String Convoyeur::getDirectionMqtt() const {
  if (state != CONVOYEUR_MOVE) return "stop";
  if (yValue > 512 + DEADZONE) return "droite";
  if (yValue < 512 - DEADZONE) return "gauche";
  return "stop";
}
