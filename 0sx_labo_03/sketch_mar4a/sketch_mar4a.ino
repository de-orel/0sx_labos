const int photoresistance = A0;
const int LED = 8;

// projecteurNocturne
int debutChronoLow = 0;
int debutChronoHigh = 0;
const int delay = 5000;
// joystickControl
const int xPin = A1;
const int yPin = A2;
const int bouton = 2;
int xValue;
int yValue;
float hauteur = 0.0;
float angle = 0.0;
unsigned long lastUpdate = 0;
// serialTransmission
const long etd = 2449427;
int previousUpdate = 0;

void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  pinMode(bouton, INPUT_PULLUP);
}

int projecteurNocturne(){
  int value = analogRead(photoresistance);
  int mappedValue = map(value, 0, 1023, 0, 100);
  int numState = 0;
  String state = "";

  if(mappedValue < 30){

    if(debutChronoLow == 0){
      debutChronoLow = millis();
    }
    if(millis() - debutChronoLow >= delay){
      digitalWrite(LED, HIGH);
      state = "ON";
      numState = 1;
    }
    debutChronoHigh = 0;

  }else{

    if(debutChronoHigh == 0){
      debutChronoHigh = millis();
    }
    if(millis() - debutChronoHigh >= delay){
      digitalWrite(LED, LOW);
      state = "OFF";
      numState = 0;
    }
    debutChronoLow = 0;
  }
  lcd.setCursor(0, 0);
  lcd.print("LUMI: ")
  lcd.print(mappedValue);
  lcd.print("%  ");

  lcd.setCursor(0, 1);
  lcd.print("LIGHT: ")
  lcd.print(state);
  lcd.print(" ");

  return numState;
}

void joystickControl(){

  int boutonValue = digitalRead(bouton)
  xValue = analogRead(xPin);
  yValue = analogRead(yPin);
  const int deadzone = 25;
  String position = "CENTER";

  xmappedValue = map(xValue, 0, 1023, -90, 90);
  unsigned long currentTime = millis();

  if(currentTime - lastUpdate >= 100){
    lastUpdated = currentTime;

    if(yValue > (512 + deadzone)){
      hauteur += 1 * 0.1;  // 100ms = 0.1s (on vérifie toutes les 0.1s si y augmente et on ajoute 1m/s * 0.1s = 0.1m)
      position = "UP";
    }
    else if(yValue < (512 - deadzone)){
      hauteur -= 1 * 0.1;
      position = "DOWN";
    }

    if(hauteur < 0) hauteur = 0;
    if(hauteur > 200) hauteur = 200;

    lcd.setCursor(0, 0);
    lcd.print("ALT: ");
    lcd.print(hauteur);
    lcd.print("m  ");

    lcd.setCursor(0, 1);
    lcd.print("DIR: ");
    lcd.print(xmappedValue);
    lcd.print("(G)  ");

  }

}

void BoutonChanger(){
  static int compteur = 0;

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
      if(compteur % 2 == 0){
        joystickControl();
        compteur++;
      }
      else{
        projecteurNocturne();
        compteur++;
      }
    }
  }
  etatPrecedent = etatPresent;
}

void serialTransmission(){
  int delay = 100;
  int valSys = projecteurNocturne();
  
  int currentTime = millis();
  if(currentTime - previousTime >= delay){
    Serial.print("etd" + String(etd));
    Serial.print("x:" + String(xValue) + ",y:" + String(yValue) + ",sys:" + String(valSys));
  }
}


void loop() {

  BoutonChanger();
  serialTransmission();
}


