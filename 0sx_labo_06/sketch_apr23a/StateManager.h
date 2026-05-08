#pragma once
#include <Arduino.h>
#include "matrice.h"
#include "lcd.h"
#include "receiver.h"

enum State { DISCOUNT, NORMAL, ERROR, CLOSE, EMERGENCY };

class StateManager {
private:
  State state;
  State lastState;
  bool isEmergency;
  Matrice&  matrice;
  Lcd& lcd;
  Receiver& receiver;

  void findStateByCode(unsigned long code);
  void findStateByInput();
  void updateDisplay();

public:
  StateManager(Matrice& m, Lcd& l, Receiver& r);
  void update();
  void handleEmergency();
  State getState() const;
};