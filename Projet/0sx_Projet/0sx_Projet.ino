#include <Arduino.h>
#include <OneButton.h>
#include <HCSR04.h>

// Labo 4 - Porte automatique
#include "Motor.h"
#include "Door.h"

// Labo 5 - Convoyeur
#include "MoteurDC.h"
#include "Convoyeur.h"

// Labo 6 - Affichage matrice
#include "matrice.h"
#include "receiver.h"
#include "remote.h"
#include "StateManager.h"

// LCD partagé
#include "lcd.h"

// Phase 5 - MQTT
#include "MqttManager.h"

// Broches Labo 4 - Porte
const int TRIGGER_PIN = 3;
const int ECHO_PIN = 4;
const int SERVO_PIN = 9;
const int BTN_DOOR_OPEN = 7;

// Broches Labo 5 - Convoyeur 
const int PIN_IN1 = 44;
const int PIN_IN2 = 45;
const int PIN_LED_MOVE = 6;     
const int PIN_LED_EMERGENCY = 5; 
const int PIN_JOY_X = A1;
const int PIN_JOY_Y = A2;

// Broches Labo 6 - Matrice / IR 
const int MATRICE_CLK = 10;
const int MATRICE_DIN = 11;
const int MATRICE_CS = 12;
const int IR_PIN = 2;

// Bouton urgence global (porte + convoyeur + magasin) 
const int BTN_EMERGENCY = 8;

// Objets Labo 4 
HCSR04 hc(TRIGGER_PIN, ECHO_PIN);
Motor servoMotor(SERVO_PIN);
Door myDoor(servoMotor, hc);
OneButton btnDoorOpen(BTN_DOOR_OPEN, true);

// Objets Labo 5 
MoteurDC dcMotor(PIN_IN1, PIN_IN2);
Convoyeur convoyeur(dcMotor, PIN_JOY_X, PIN_JOY_Y, PIN_LED_MOVE, PIN_LED_EMERGENCY);

// Objets Labo 6 
Matrice myMatrice(MATRICE_CLK, MATRICE_DIN, MATRICE_CS);
Receiver myReceiver(IR_PIN);
Lcd myLcd;
StateManager stateManager(myMatrice, myLcd, myReceiver);

// Bouton urgence global
OneButton btnEmergency(BTN_EMERGENCY, true);

// Phase 5 - MQTT 
MqttManager mqttManager(stateManager);

// LCD - Cyclage d'affichage 
// Vue 0 : Porte (Labo 4)
// Vue 1 : Convoyeur (Labo 5)
// Vue 2 : Mode magasin (Labo 6)
// En urgence : affichage fixe urgence uniquement
const unsigned long LCD_CYCLE_DELAY = 4000;
unsigned long lastLcdCycle = 0;
int lcdViewIndex = 0;
const int LCD_VIEW_COUNT = 3;

// Callbacks boutons
void onDoorOpen() {
  myDoor.openWithButton();
}

// Bouton 8 : active/desactive l'urgence sur tous les systemes
void onEmergency() {
  myDoor.toggleEmergency();
  convoyeur.toggleEmergency();
  stateManager.handleEmergency();
}

// Prototypes
void updateLcdCycle();
void mqttTask();


void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  // Labo 4
  servoMotor.begin();
  btnDoorOpen.attachClick(onDoorOpen);

  // Labo 5
  dcMotor.begin();
  convoyeur.begin();

  // Labo 6
  myMatrice.begin();
  myReceiver.begin();

  // Urgence globale
  btnEmergency.attachClick(onEmergency);

  // LCD
  myLcd.init();

  // MQTT + WiFi
  mqttManager.begin();
}

void loop() {
  // Boutons 
  btnDoorOpen.tick();
  btnEmergency.tick();

  // Labo 4 
  myDoor.update();

  // Labo 5 
  convoyeur.update();

  // Labo 6 
  stateManager.update();
  stateManager.updateMatrice();

  // LCD cyclique
  updateLcdCycle();

  // MQTT
  mqttTask();
}

// Tâche MQTT 
void mqttTask() {
  mqttManager.update();
  mqttManager.sendState(
    convoyeur.getDirectionMqtt(),
    convoyeur.getSpeed(),
    myDoor.getPorteLabel(),
    myDoor.getClientCount());
}

// Cyclage LCD 
// En mode urgence : affichage fixe, pas de défilement
void updateLcdCycle() {
  if (stateManager.getState() == EMERGENCY_STORE) {
    myLcd.printEmergency();
    return;
  }

  if (millis() - lastLcdCycle >= LCD_CYCLE_DELAY) {
    lastLcdCycle = millis();
    myLcd.clean();
    lcdViewIndex = (lcdViewIndex + 1) % LCD_VIEW_COUNT;
  }

  switch (lcdViewIndex) {
    case 0:
      myLcd.printDoorState(myDoor.getStatue());
      break;
    case 1:
      myLcd.printConvoyeurInfo(
        convoyeur.getActiveLabel(),
        convoyeur.getPositionLabel(),
        convoyeur.getSpeed());
      break;
    case 2:
      switch (stateManager.getState()) {
        case DISCOUNT: myLcd.printDiscount(); break;
        case NORMAL: myLcd.printNormal(); break;
        case ERROR: myLcd.printError(); break;
        case CLOSE_STORE: myLcd.printClose(); break;
        default: break;
      }
      break;
  }
}
