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

OneButton buttonOpening;
OneButton buttonEmergency;

LCD_I2C lcd(0x27, 16, 2);

void setup() {

  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();

  myServo.attach(SERVO_PIN);
  buttonOpening.setup(BTN_OPENING, INPUT_PULLUP, true);
  buttonEmergency.setup(BTN_EMERGENCY, INPUT_PULLUP, true);

}

enum Etat{
  CLOSE,
  OPENING,
  OPEN,
  CLOSING
};
Etat state = CLOSE;

void loop() {
  autoOpening();

  buttonOpening.tick();
  buttonEmergency.tick();
  openWithButton();
  emergencybutton();
}

void autoOpening(){
  const float AREA1 = 0;
  const float AREA2 = 30;
  float distance = hc.dist();
  static unsigned long lastMove = 0;
  static int angle = 10;

  static long valid = 0;

  Serial.println(distance);

  switch(state){

    case CLOSE:
      if(distance > AREA1 && distance <= AREA2){
        state = OPENING;
      }
      lcd.clear();
    break;

    case OPENING:
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
      lcd.print("OUVERTURE..");
    break;

    case OPEN:
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
    break;

    case CLOSING:
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
      lcd.print("FERMETURE..");
    break;
  }
}


void openWithButton(){

  if(digitalRead(BTN_OPENING) == 0 && state != OPENING){
    state = OPENING;
  }
}

void emergencybutton(){

  static bool active = true;

  if(digitalRead(BTN_EMERGENCY) == 0 && active){
    myServo.detach();
    active = false;
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("!! URGENCE !!");
  }
  
  if(digitalRead(BTN_EMERGENCY) == 0 && !active){
    myServo.attach(3);
    state = CLOSING;
    active = true;
  } 
}