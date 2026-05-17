#include "Door.h"

Door::Door(Motor& m, HCSR04& s)
  : motor(m), sensor(s), state(CLOSE), distance(0), lastDate(0)
{}

void Door::update() {
  if (millis() - lastDate >= 50) {
    lastDate = millis();
    distance = sensor.dist();
  }

  if (state == EMERGENCY) return;

  switch (state) {
    case CLOSE:
      checkDistance();
      break;
    case OPENING:
      if (motor.openDoor()) state = OPEN;
      break;
    case OPEN:
      if (motor.waitDelay()) state = CLOSING;
      break;
    case CLOSING:
      checkDistance();
      if (motor.closeDoor()) state = CLOSE;
      break;
  }
}

void Door::checkDistance() {
  if (distance > AREA1 && distance <= AREA2) {
    state = OPENING;
  }
}

void Door::openWithButton() {
  if (state != OPENING) state = OPENING;
}

void Door::toggleEmergency() {
  static bool active = true;
  if (active) {
    motor.detach();
    state  = EMERGENCY;
    active = false;
  } else {
    motor.attach();
    state  = CLOSING;
    active = true;
  }
}

String Door::getStatue() const {
  switch (state) {
    case CLOSE:     return "FERMEE !     ";
    case OPENING:   return "OUVERTURE.. ";
    case OPEN:      return "OUVERTE !   ";
    case CLOSING:   return "FERMETURE.. ";
    case EMERGENCY: return "!! URGENCE !!";
    default:        return "";
  }
}