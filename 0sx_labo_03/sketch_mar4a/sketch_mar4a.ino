#include <Wire.h>
#include <LCD_I2C.h>
#include <LiquidCrystal_I2C.h>

const int photoresistance = A0;
const int LED = 8;

// projecteurNocturne
unsigned long debutChronoLow = 0;
unsigned long debutChronoHigh = 0;
// joystickControl
const int xPin = A1, yPin = A2, bouton = 2;
int xValue;
int yValue;
unsigned long lastUpdate = 0;
// serialTransmission
const long etd = 2449427;
int previousUpdate = 0;
// pages LCD
int page = 0;

LCD_I2C lcd(0x27, 16, 2);
byte special[8] = {B11100, B00100, B01000, B11100, B00111, B00001, B00010, B00100};  // caractère spécial

void setup() {

  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  pinMode(bouton, INPUT_PULLUP);

  lcd.begin();
  lcd.backlight();
  lcd.createChar(0, special);
}

int projecteurNocturne(){

  const unsigned long  delayy = 5000;
  int value = analogRead(photoresistance);
  int mappedValue = map(value, 0, 1023, 0, 100);
  int numState = 0;
  String state = "";

  if(mappedValue < 30){

    if(debutChronoLow == 0){
      debutChronoLow = millis();
    }
    if(millis() - debutChronoLow >= delayy){
      digitalWrite(LED, HIGH);
      state = "ON ";
      numState = 1;
    }
    debutChronoHigh = 0;

  }else{

    if(debutChronoHigh == 0){
      debutChronoHigh = millis();
    }
    if(millis() - debutChronoHigh >= delayy){
      digitalWrite(LED, LOW);
      state = "OFF";
      numState = 0;
    }
    debutChronoLow = 0;
  }
    if(page == 0){
      lcd.setCursor(0, 0);
      lcd.print("LUMI: ");
      lcd.print(mappedValue);
      lcd.print("%  ");

      lcd.setCursor(0, 1);
      lcd.print("LIGHT: ");
      lcd.print(state);
  }

  return numState;
}

void joystickControl(){

  static float hauteur = 0.0;
  static float angle = 0.0;
  int boutonValue = digitalRead(bouton);

  xValue = analogRead(xPin);
  yValue = analogRead(yPin);
  const int deadzone = 25;
  String position = "CENTER";

  int xmappedValue = map(xValue, 0, 1023, -90, 90);
  unsigned long currentTime = millis();

  if(currentTime - lastUpdate >= 100){
    lastUpdate = currentTime;

    if(yValue > (512 + deadzone)){
      hauteur += 1 * 0.1;  // 100ms = 0.1s (on vérifie toutes les 0.1s si la hauteur augmente et on ajoute 1m/s * 0.1s = 0.1m)
      position = "UP";
    }
    else if(yValue < (512 - deadzone)){
      hauteur -= 1 * 0.1;
      position = "DOWN";
    }

    if(hauteur < 0) hauteur = 0;
    if(hauteur > 200) hauteur = 200;

    if(page == 1){
      lcd.setCursor(0, 0);
      lcd.print("ALT: ");
      lcd.print(hauteur);
      lcd.print("m  ");

      lcd.setCursor(0, 1);
      lcd.print("DIR: ");
      lcd.print(xmappedValue);
      if(xmappedValue < 0)lcd.print("(G)  ");
      else lcd.print("(D)  ");      
    }
  }
}

void BoutonChanger(){;

  static int etatPrecedent = HIGH;
  static int etat = HIGH; 
  const int delai = 50; 
  static unsigned long dernierChangement = 0; 

  int etatPresent = digitalRead(bouton);

  if (etatPresent != etatPrecedent) { 
    dernierChangement = millis();
  }

  if ((millis() - dernierChangement) > delai && etatPresent != etat) { 
    etat = etatPresent; 
    if(digitalRead(bouton) == 0){
      page = !page;
      lcd.clear();
    }
  }
  etatPrecedent = etatPresent;
}

void serialTransmission(){

  int delaie = 1000;
  int valSys = projecteurNocturne();
  
  int currentTime = millis();
  if(currentTime - previousUpdate >= delaie){
    Serial.print("etd" + String(etd));
    Serial.print("x:" + String(xValue) + ",y:" + String(yValue) + ",sys:" + String(valSys));
    Serial.println();
  }
}


void loop() {

  const int delai = 3000;
  unsigned long time = millis();
  static bool clean = true;
  if(time <= delai){
    lcd.setCursor(0, 0);
    lcd.print("NGUEDIA DEMANOU");

    lcd.setCursor(0, 1);
    lcd.write(0);

    lcd.setCursor(9, 1);
    lcd.print("*****27");
  }else{
    if(clean){
     lcd.clear();
     clean = false; 
    }
    projecteurNocturne();
    joystickControl();
    BoutonChanger();
    serialTransmission();
  }
}


