#include "MoteurDC.h"

MoteurDC::MoteurDC(int p1, int p2) {
  this->pin1 = p1;
  this->pin2 = p2;
  this->speedValue = 60;
}

void MoteurDC::begin() {
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  shutDown();
}

void MoteurDC::applySpeed() {
  if (digitalRead(pin1) == HIGH) {
    analogWrite(pin1, speedValue);
    digitalWrite(pin2, LOW);
  } else if (digitalRead(pin2) == HIGH) {
    digitalWrite(pin1, LOW);
    analogWrite(pin2, speedValue);
  }
}

void MoteurDC::moveForward() {
  digitalWrite(pin1, HIGH);
  digitalWrite(pin2, LOW);
  applySpeed();
}

void MoteurDC::moveBackward() {
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, HIGH);
  applySpeed();
}

void MoteurDC::increaseSpeed() {
  if (this->speedValue < 60) this->speedValue = 60;
  else if (this->speedValue <= 250) this->speedValue += 5;
  applySpeed();
}

void MoteurDC::decreaseSpeed() {
  if (this->speedValue > 60) this->speedValue -= 5;
  else this->speedValue = 60;
  applySpeed();
}

void MoteurDC::shutDown() {
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, LOW);
}

int MoteurDC::getSpeed() {
  return this->speedValue;
}
