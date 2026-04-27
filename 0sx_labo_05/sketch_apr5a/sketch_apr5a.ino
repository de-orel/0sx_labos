#include <OneButton.h>
#include <LCD_I2C.h>
#include "Moteur_dc.h"

const int PIN_IN1 = 44, PIN_IN2 = 45;
const int PIN_LED_MOVE = 9, PIN_LED_EMERGENCY = 8;
const int PIN_BTN_EMERGENCY = 7;
const int PIN_JOY_X = A1, PIN_JOY_Y = A2;
const int DEADZONE = 50;

LCD_I2C lcd(0x27, 16, 2);
Motor motor(PIN_IN1, PIN_IN2);
OneButton emergencyBtn(PIN_BTN_EMERGENCY, true);

enum Etat { REST, MOVE, EMERGENCY };
Etat state = REST;

String activeLabel = "NON", positionLabel = "STATIONNAIRE";
int xValue, yValue;
unsigned long lastSpeedUpdate = 0;

void setup() {
    motor.begin();
    lcd.begin();
    lcd.backlight();
    pinMode(PIN_LED_MOVE, OUTPUT);
    pinMode(PIN_LED_EMERGENCY, OUTPUT);

    emergencyBtn.attachClick(toggleEmergency);
}

void loop() {
    emergencyBtn.tick();

    switch (state) {
        
        case REST:
            handleRestState();
            break;

        case MOVE:
            handleMoveState();
            break;

        case EMERGENCY:
            handleEmergencyState();
            break;
    }
}


void handleRestState() {
    motor.shutDown();
    digitalWrite(PIN_LED_MOVE, LOW);
    digitalWrite(PIN_LED_EMERGENCY, LOW);
    activeLabel = "NON";
    positionLabel = "STATIONNAIRE";
    
    checkJoystick(); 
    updateDisplay();
}

void handleMoveState() {
    digitalWrite(PIN_LED_MOVE, HIGH);
    digitalWrite(PIN_LED_EMERGENCY, LOW);
    activeLabel = "OUI";
    
    controlMovement(); 
    controlSpeed(); 
    checkJoystick(); 
    updateDisplay();
}

void handleEmergencyState() {
    motor.shutDown();
    digitalWrite(PIN_LED_MOVE, LOW);
    digitalWrite(PIN_LED_EMERGENCY, HIGH);
    displayEmergencyAlert();
}


void checkJoystick() {
    yValue = analogRead(PIN_JOY_Y);
    
    if (abs(yValue - 512) > DEADZONE) {
        state = MOVE;
    } else {
        state = REST;
    }
}

void controlMovement() {
    if (yValue > 512 + DEADZONE) {
        motor.moveForward();
        positionLabel = "AVANT";
    } else if (yValue < 512 - DEADZONE) {
        motor.moveBackward();
        positionLabel = "ARRIERE";
    }
}

void controlSpeed() {
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

void toggleEmergency() {
    lcd.clear();
    if (state == EMERGENCY) {
        state = REST;
    } else {
        state = EMERGENCY;
    }
}

void displayEmergencyAlert() {
    lcd.setCursor(0, 0);
    lcd.print(" !! URGENCE !!  ");
    lcd.setCursor(0, 1);
    lcd.print(" PRESS TO RESET ");
}

void updateDisplay() {
    static unsigned long lastRefresh = 0;
    static int topIndex = 0;
    
    if (millis() - lastRefresh >= 2500) {
        lastRefresh = millis();
        lcd.clear();
        
        String lines[3];
        lines[0] = "Actif: " + activeLabel;
        lines[1] = "Sens: " + positionLabel;
        lines[2] = "Vit: " + String(activeLabel == "OUI" ? motor.getSpeed() : 0);

        lcd.setCursor(0, 0);
        lcd.print(lines[topIndex]);
        lcd.setCursor(0, 1);
        lcd.print(lines[(topIndex + 1) % 3]);

        topIndex = (topIndex + 1) % 3;
    }
}