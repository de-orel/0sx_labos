#pragma once
#include <Arduino.h>

class Motor {
private:
    int pin1;
    int pin2;
    int speedValue;
    void applySpeed(); 

public:
    Motor(int p1, int p2);
    void begin();
    void moveForward();
    void moveBackward();
    void increaseSpeed();
    void decreaseSpeed();
    void shutDown();
    int getSpeed();
};