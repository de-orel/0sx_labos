#include "Motor.h"

Motor::Motor(int pin)
  : servoPin(pin), angle(10), lastMove(0), lastOpening(0)
{}

void Motor::begin() {
  servo.write(angle);
  servo.attach(servoPin);
}

void Motor::attach() {
  servo.attach(servoPin);
}

void Motor::detach() {
  servo.detach();
}

bool Motor::openDoor() {
  if (millis() - lastMove >= 15) {
    lastMove = millis();
    if (angle < 170) {
      angle++;
      servo.write(angle);
    } else {
      lastOpening = millis();
      return true;
    }
  }
  return false;
}

bool Motor::closeDoor() {
  if (millis() - lastMove >= 15) {
    lastMove = millis();
    if (angle > 0) {
      angle--;
      servo.write(angle);
    } else {
      return true;
    }
  }
  return false;
}

bool Motor::waitDelay() {
  return (millis() - lastOpening >= 10000);
}