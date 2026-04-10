#include <HCSR04.h>
#include <Servo.h>
#include "OneButton.h"
#include <LCD_I2C.h>

#define TRIGGER_PIN 9
#define ECHO_PIN 10
#define SERVO_PIN 3

// Objet de type capteur de distance
HCSR04 hc(TRIGGER_PIN, ECHO_PIN);

// Objet de type servo
Servo myServo;
unsigned long lastOpening = 0;

// Les boutons
const int BTN_OPENING = 7;
const int BTN_EMERGENCY = 8;

const float AREA1 = 0;
const float AREA2 = 30;
float distance = 0;
unsigned long lastMove = 0;
int angle = 10;
unsigned long lastDate = 0;

OneButton buttonOpening(BTN_OPENING, true);
OneButton buttonEmergency(BTN_EMERGENCY, true);

LCD_I2C lcd(0x27, 16, 2);

void setup() {

  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  myServo.write(angle);

  myServo.attach(SERVO_PIN);
  buttonOpening.attachClick(openWithButton);
  buttonEmergency.attachClick(emergencybutton);
}

enum Etat{
  CLOSE,
  OPENING,
  OPEN,
  CLOSING,
  EMERGENCY
};
Etat state = CLOSE;

void loop() {
  buttonOpening.tick();
  buttonEmergency.tick();

  if(millis() - lastDate >= 50){
    lastDate = millis();
    distance = hc.dist();
  }
  autoOpening();
}

unsigned long valid = 0;

void checkDistance(){
  if(distance > AREA1 && distance <= AREA2){
    state = OPENING;
  }
}

void openDoor(){

  if(millis() - lastMove >= 15){
    lastMove = millis();
    if(angle < 170){
      angle++;
      myServo.write(angle);
    }
    else{
      lastOpening = millis();
      state = OPEN;
    }
  }
  lcd.setCursor(1, 0);
  lcd.print("OUVERTURE..  ");
}

void waitDelay(){

  if(millis() - valid >= 1000){
    valid = millis();
    lcd.setCursor(1, 1);
    int timeSecond = (millis() - lastOpening) / 1000.0; 
    lcd.print(timeSecond);
  }
  if(millis() - lastOpening >= 10000){
    state = CLOSING;
    lcd.clear();
  }
}

void closeDoor(){

  checkDistance();

  if(millis() - lastMove >= 15){
    lastMove = millis();
    if(angle > 0){
      angle--;
      myServo.write(angle);
    }
    else{
      state = CLOSE;
    }
  }
  lcd.setCursor(1, 0);
  lcd.print("FERMETURE..  ");
}


void autoOpening(){
  if(state == EMERGENCY){
    return;
  }
  Serial.println(distance);
  switch(state){

    case CLOSE:
      checkDistance();
      break;
    case OPENING:
      openDoor();
      break;
    case OPEN:
      waitDelay();
      break;
    case CLOSING:
      closeDoor();
      break;
  }
}


void openWithButton(){

  if(state != OPENING){
    state = OPENING;
  }
}

void emergencybutton(){

  static bool active = true;
  if(active == true){
    myServo.detach();
    active = false;
    state = EMERGENCY;
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("!! URGENCE !!");
  }
  else if(active == false){
    myServo.attach(SERVO_PIN);
    state = CLOSING;
    active = true;
  } 
}