#include "Door.h"

Door::Door(Motor& m, HCSR04& s)
  : motor(m), sensor(s), state(PORTE_CLOSE), distance(0), lastDate(0), clientCount(0) {}

void Door::update() {
  if (millis() - lastDate >= 50) {
    lastDate = millis();
    distance = sensor.dist();
  }

  if (state == PORTE_EMERGENCY) return;

  switch (state) {
    case PORTE_CLOSE:
      checkDistance();
      break;
    case PORTE_OPENING:
      if (motor.openDoor()) state = PORTE_OPEN;
      break;
    case PORTE_OPEN:
      if (motor.waitDelay()) state = PORTE_CLOSING;
      break;
    case PORTE_CLOSING:
      checkDistance();
      if (motor.closeDoor()) state = PORTE_CLOSE;
      break;
  }
}

void Door::checkDistance() {
  if (distance > AREA1 && distance <= AREA2) {
    clientCount++;
    state = PORTE_OPENING;
  }
}

void Door::openWithButton() {
  if (state != PORTE_OPENING) {
    clientCount++;
    state = PORTE_OPENING;
  }
}

void Door::toggleEmergency() {
  static bool active = true;
  if (active) {
    motor.detach();
    state = PORTE_EMERGENCY;
    active = false;
  } else {
    motor.attach();
    state = PORTE_CLOSING;
    active = true;
  }
}

String Door::getStatue() const {
  switch (state) {
    case PORTE_CLOSE: return "FERMEE !     ";
    case PORTE_OPENING: return "OUVERTURE.. ";
    case PORTE_OPEN: return "OUVERTE !   ";
    case PORTE_CLOSING: return "FERMETURE.. ";
    case PORTE_EMERGENCY: return "!! URGENCE !!";
    default: return "";
  }
}

// Pour MQTT : move / ouvert / ferme
String Door::getPorteLabel() const {
  switch (state) {
    case PORTE_OPENING:
    case PORTE_CLOSING: return "move";
    case PORTE_OPEN: return "ouvert";
    case PORTE_CLOSE: return "ferme";
    case PORTE_EMERGENCY: return "ferme";
    default: return "ferme";
  }
}

int Door::getClientCount() const {
  return clientCount;
}
