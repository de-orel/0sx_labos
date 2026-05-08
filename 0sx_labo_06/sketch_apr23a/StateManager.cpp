#include "StateManager.h"
#include "remote.h"

StateManager::StateManager(Matrice& m, Lcd& l, Receiver& r)
  : matrice(m), lcd(l), receiver(r),
    state(NORMAL), lastState(CLOSE), isEmergency(false)
{}

void StateManager::update() {
  if (state != EMERGENCY) {
    unsigned long code = receiver.getCode();
    static unsigned long lastCode = 0;
    if (code != 0 && code != lastCode) {
      lastCode = code;
      findStateByCode(code);
      updateDisplay();
      return;
    }
    findStateByInput();
  }
  updateDisplay();
}

void StateManager::handleEmergency() {
  isEmergency = !isEmergency;
  state = isEmergency ? EMERGENCY : NORMAL;
}

State StateManager::getState() const {
  return state;
}

void StateManager::findStateByCode(unsigned long code) {
  if (code == Remote::DISCOUNT) state = DISCOUNT;
  else if (code == Remote::NORMAL)   state = NORMAL;
  else if (code == Remote::ERROR)    state = ERROR;
  else if (code == Remote::CLOSE)    state = CLOSE;
}

void StateManager::findStateByInput() {
  if (Serial.available() > 0) {
    int cmd = Serial.read();
    if (cmd == '1') state = DISCOUNT;
    else if (cmd == '2') state = NORMAL;
    else if (cmd == '3') state = ERROR;
    else if (cmd == '4') state = CLOSE;
  }
}

void StateManager::updateDisplay() {
  if (state != lastState) {
    lcd.clean();
    matrice.clean();
    switch (state) {
      case DISCOUNT: lcd.printDiscount(); break;
      case NORMAL: lcd.printNormal(); break;
      case ERROR: lcd.printError(); break;
      case CLOSE: lcd.printClose(); break;
      case EMERGENCY: lcd.printEmergency(); break;
    }
    lastState = state;
  }
}