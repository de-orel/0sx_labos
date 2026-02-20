const int ledPins[] = {7, 8, 9, 10};
int potentiometerPin = A1;
int potentiometerValue = 0;
const int bouton = 2;

const unsigned long debounceDelay = 50;
unsigned long lastDebounceTime = 0;
int lastButtonState = HIGH;
int buttonState;

void setup() {

  Serial.begin(9600);
  for (int i = 0; i < 4; i++){
    pinMode(ledPins[i], OUTPUT);
  }
  pinMode(bouton, INPUT_PULLUP);

}

void loop() {
  
  potentiometerValue = analogRead(potentiometerPin);
  int converted_value = map(potentiometerValue, 0, 1023, 0, 20);
  int pourcentage = map(converted_value, 0, 20, 0, 100);

  showLevel(converted_value, pourcentage);
  printLeds(pourcentage);
}


void showLevel(int converted_value, int pourcentage){           // N.A: 2449427      avant dernier chiffre(pair): 2

  int reading = digitalRead(bouton);

  if (reading != lastButtonState){
    lastDebounceTime = millis();
  }
  if (millis() - lastDebounceTime >= debounceDelay){
    if(reading != buttonState){
      buttonState = reading;
    
      if (digitalRead(bouton) == 0){
        Serial.print("[");
        for(int i = 0; i < converted_value; i++){
          Serial.print("!");
        }
        for (int i = 0; i < (20 - converted_value); i++){
          Serial.print(".");
        }
        Serial.println("]   " + String(pourcentage) + "%");
      }
    }
  }
  lastButtonState = reading;
}

void printLeds(int pourcentage){          // N.A: 2449427       dernier chiffre(impair): 7

  if (pourcentage <= 25){
    digitalWrite(ledPins[0], HIGH);
    digitalWrite(ledPins[1], LOW);
    digitalWrite(ledPins[2], LOW);
    digitalWrite(ledPins[3], LOW);
  }
  else if(pourcentage <= 50){
    digitalWrite(ledPins[0], HIGH);
    digitalWrite(ledPins[1], HIGH);
    digitalWrite(ledPins[2], LOW);
    digitalWrite(ledPins[3], LOW);
  }
  else if(pourcentage <= 75){  
    digitalWrite(ledPins[0], HIGH);
    digitalWrite(ledPins[1], HIGH);
    digitalWrite(ledPins[2], HIGH);
    digitalWrite(ledPins[3], LOW);
  }
  else {
    digitalWrite(ledPins[0], HIGH);
    digitalWrite(ledPins[1], HIGH);
    digitalWrite(ledPins[2], HIGH);
    digitalWrite(ledPins[3], HIGH);
  }
}
