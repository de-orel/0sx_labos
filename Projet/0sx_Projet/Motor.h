#pragma once
#include <Arduino.h>
#include <Servo.h>

class Motor {
private:
  Servo servo;
  int servoPin;
  int angle;
  unsigned long lastMove;
  unsigned long lastOpening;

public:
  Motor(int pin);
  void begin();
  void attach();
  void detach();
  bool openDoor();
  bool closeDoor();
  bool waitDelay();
};
