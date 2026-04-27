#include "Moteur_dc.h"

Motor::Motor(int p1, int p2) {
    this->pin1 = p1;
    this->pin2 = p2;
    this->speedValue = 60; // Initial speed set to 60
}

void Motor::begin() {
    pinMode(pin1, OUTPUT);
    pinMode(pin2, OUTPUT);
    shutDown();
}

void Motor::applySpeed() {
    if (digitalRead(pin1) == HIGH) {
        analogWrite(pin1, speedValue);
        digitalWrite(pin2, LOW);
    } else if (digitalRead(pin2) == HIGH) {
        digitalWrite(pin1, LOW);
        analogWrite(pin2, speedValue);
    }
}

void Motor::moveForward() {
    digitalWrite(pin1, HIGH);
    digitalWrite(pin2, LOW);
    applySpeed();
}

void Motor::moveBackward() {
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, HIGH);
    applySpeed();
}

void Motor::increaseSpeed() {
    if (this->speedValue < 60) this->speedValue = 60;
    else if (this->speedValue <= 250) this->speedValue += 5; 
    applySpeed();
}

void Motor::decreaseSpeed() {
    if (this->speedValue > 60) this->speedValue -= 5;
    else this->speedValue = 60; 
    applySpeed();
}

void Motor::shutDown() {
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, LOW);
}

int Motor::getSpeed() {
    return this->speedValue;
}