#include "OneButton.h"
#include <LCD_I2C.h>
// Moteur DC
const int INPUT_1 = 44;
const int INPUT_2 = 45;
// Led
const int LED_1 = 7;
const int LED_2 = 8;
// Bouton
const int EMERG_BTN = 9;
// joystickControl
const int X_PIN = A1
const int Y_PIN = A2
const int JOY_BTN = 2;
const int DEADZONE = 25;
int xValue, yValue;
int val = 0;
// Lcd
LCD_I2C lcd(0x27, 16, 2);
String state = "", position = "";

OneButton buttonEmergency;

void setup() {
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();

  pinMode(INPUT_1, OUTPUT);
  pinMode(INPUT_2, OUTPUT);
  pinMode(JOY_BTN, INPUT_PULLUP);  
  buttonEmergency.setup(EMERG_BTN, INPUT_PULLUP, true);
}

void loop() {
  buttonEmergency.tick();
}

void forwardBackward(){

  yValue = analogRead(Y_PIN);
  if(yValue > 512 + DEADZONE){
    digitalWrite(INPUT_1, HIGH);
    digitalWrite(INPUT_2, LOW);
    digitalWrite(LED_1, HIGH);
    active = "OUI";
    Position = "AVANT";
  }
  else if(yValue < 512 - DEADZONE){
    digitalWrite(INPUT_1, LOW);
    digitalWrite(INPUT_2, HIGH);
    digitalWrite(LED_1, HIGH);
    active = "OUI"; 
    postion = "ARRIÈRE"; 
  }
  else{
    digitalWrite(INPUT_1, LOW);
    digitalWrite(INPUT_2, LOW);
    digitalWrite(LED_1, LOW);
    active = "NON";
    position = "CENTER";
  }
}

void speedControl(){

  static long lastMove = 0;
  xValue = analogRead(X_PIN);
  if(xValue > 512 + DEADZONE){
    if(millis() - lastMove >= 10){
      lastMove = millis();
      if(val < 255){
        val++;
        analogWrite(INPUT_1, val);
        analogWrite(INPUT_2, 0);
        digitalWrite(LED_1, HIGH);
      }
    }
  }
  else if(xValue < 512 - DEADZONE){
    if(millis() - lastMove >= 10){
      lastMove = millis();
      if(val > 0){
        val--;
        analogWrite(INPUT_1, 0);
        analogWrite(INPUT_2, val);
        digitalWrite(LED_1, HIGH);
      }  
    }
  }
  else{
    digitalWrite(INPUT_1, LOW);
    digitalWrite(INPUT_2, LOW);
    digitalWrite(LED_1, LOW);
  } 
}


void screenPrinter(){

// On prépare les 3 lignes de texte dans un tableau
  static long lastRefresh = 0;
  String lignes[3];
  lignes[0] = "Actif: " + active;
  lignes[1] = "Sens: " + position;
  lignes[2] = "Vitesse: " + String(val) + " %";

  for (int i = 0; i < 3; i++) {
    if(millis() - lastRefresh >= 2500){
      lastRefresh = millis();
      lcd.clear();
      
      // Affiche l'info actuelle sur la ligne du haut (0)
      lcd.setCursor(0, 0);
      lcd.print(lignes[i]);
      
      // Affiche l'info suivante sur la ligne du bas (1)
      // Le "% 3" permet de revenir à la première info après la troisième
      int next = (i + 1) % 3;
      lcd.setCursor(0, 1);
      lcd.print(lignes[next]);

    }
  }

}

void emergencyStop(){

}
