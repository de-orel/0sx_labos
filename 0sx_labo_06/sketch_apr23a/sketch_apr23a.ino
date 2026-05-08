#include <Arduino.h>
#include "OneButton.h"
#include "matrice.h"
#include "receiver.h"
#include "lcd.h"
#include "StateManager.h"

const int PIN_BTN_EMERGENCY = 7;

Matrice myMatrice(10, 11, 12);
Receiver myReceiver(2);
Lcd myLcd;
StateManager stateManager(myMatrice, myLcd, myReceiver);
OneButton emergencyBtn(PIN_BTN_EMERGENCY, true);

void setup() {
  Serial.begin(115200);
  emergencyBtn.attachClick(handleEmergency);

  myLcd.init();
  myMatrice.begin();
  myReceiver.begin();
}

void loop() {
  emergencyBtn.tick();
  stateManager.update();

  switch (stateManager.getState()) {
    case DISCOUNT: myMatrice.scrollString("SPECIAL"); break;
    case NORMAL: myMatrice.drawSmiley(); break;
    case ERROR: myMatrice.toggleOnOff(); break;
    case CLOSE: myMatrice.shutDown(); break;
    case EMERGENCY: myMatrice.emergency(); break;
  }
}

void handleEmergency() {
  stateManager.handleEmergency();
}