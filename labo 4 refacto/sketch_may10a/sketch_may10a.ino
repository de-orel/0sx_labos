#include "OneButton.h"
#include <LCD_I2C.h>
#include "Motor.h"
#include "Door.h"

#define TRIGGER_PIN 9
#define ECHO_PIN    10
#define SERVO_PIN   3

const int BTN_OPENING   = 7;
const int BTN_EMERGENCY = 8;

HCSR04    hc(TRIGGER_PIN, ECHO_PIN);
Motor     myMotor(SERVO_PIN);
Door      myDoor(myMotor, hc);
LCD_I2C   lcd(0x27, 16, 2);

OneButton buttonOpening(BTN_OPENING, true);
OneButton buttonEmergency(BTN_EMERGENCY, true);

void setup() {
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  myMotor.begin();

  buttonOpening.attachClick(openWithButton);
  buttonEmergency.attachClick(emergencyButton);
}

void loop() {
  buttonOpening.tick();
  buttonEmergency.tick();
  myDoor.update();

  lcd.setCursor(1, 0);
  lcd.print(myDoor.getStatue());
}

void openWithButton()  { myDoor.openWithButton();  }
void emergencyButton() { myDoor.toggleEmergency(); }