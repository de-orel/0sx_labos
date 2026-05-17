#include "StateManager.h"
#include "remote.h"

StateManager::StateManager(Matrice& m, Lcd& l, Receiver& r)
  : matrice(m), lcd(l), receiver(r),
    state(NORMAL), lastState(CLOSE_STORE), isEmergency(false) {}

void StateManager::update() {
  if (state != EMERGENCY_STORE) {
    unsigned long code = receiver.getCode();
    static unsigned long lastCode = 0;
    if (code != 0 && code != lastCode) {
      lastCode = code;
      findStateByCode(code);
      return;
    }
    findStateByInput();
  }
}

void StateManager::handleEmergency() {
  isEmergency = !isEmergency;
  state = isEmergency ? EMERGENCY_STORE : NORMAL;
  lastState = CLOSE_STORE;  // forcer un refresh LCD
}

State StateManager::getState() const {
  return state;
}

void StateManager::setState(State s) {
  state = s;
  lastState = CLOSE_STORE;  // forcer un refresh LCD
}

// Pour MQTT : rabais / ok (normal+ferme) / urgence / erreur
String StateManager::getStatutLabel() const {
  switch (state) {
    case DISCOUNT: return "rabais";
    case NORMAL: return "ok";
    case ERROR: return "erreur";
    case CLOSE_STORE: return "ok";
    case EMERGENCY_STORE: return "urgence";
    default: return "ok";
  }
}

void StateManager::findStateByCode(unsigned long code) {
  if (code == Remote::DISCOUNT) state = DISCOUNT;
  else if (code == Remote::NORMAL) state = NORMAL;
  else if (code == Remote::ERROR) state = ERROR;
  else if (code == Remote::CLOSE) state = CLOSE_STORE;
}

void StateManager::findStateByInput() {
  if (Serial.available() > 0) {
    int cmd = Serial.read();
    if (cmd == '1') state = DISCOUNT;
    else if (cmd == '2') state = NORMAL;
    else if (cmd == '3') state = ERROR;
    else if (cmd == '4') state = CLOSE_STORE;
  }
}

void StateManager::updateMatrice() {
  switch (state) {
    case DISCOUNT: matrice.scrollString("SPECIAL"); break;
    case NORMAL: matrice.drawSmiley(); break;
    case ERROR: matrice.toggleOnOff(); break;
    case CLOSE_STORE: matrice.shutDown(); break;
    case EMERGENCY_STORE: matrice.emergency(); break;
  }
}
