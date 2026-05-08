#include "receiver.h"
#include <IRremote.hpp>
#include <Arduino.h>


Receiver::Receiver(int pin){
  receive_pin = pin;
}

void Receiver::begin() {
  IrReceiver.begin(receive_pin, ENABLE_LED_FEEDBACK);
}
unsigned long Receiver::getCode() {
  if (IrReceiver.decode()) {
    unsigned long received = IrReceiver.decodedIRData.decodedRawData;
    IrReceiver.resume();

    if (received == 0 || received == 0xFFFFFFFF) return 0;

    return received;
  }
  return 0;
}