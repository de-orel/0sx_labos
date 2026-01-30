const int led = 13;
const long etd = 2449427;
int brightness = 255;
int fadeAmount = -5;

void setup() {
  // put your setup code here, to run once:
  pinMode(led, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // État allumé et éteint
  Serial.print("Etat: Allume - ");
  Serial.print(etd);
  Serial.println();
  digitalWrite(led, LOW);
  delay(300);
  digitalWrite(led, HIGH);
  delay(2000);

  // État variation d'intensité
  Serial.print("Etat: Varie - ");
  Serial.print(etd);
  Serial.println();
  analogWrite(led, brightness);
  brightness = brightness + fadeAmount;
  if(brightness <= 0 || brightness >= 255){
    fadeAmount = -fadeAmount;
  }
  delay(2048);

  // État clignotement
  Serial.print("Etat: Clignotement - ");
  Serial.print(etd);
  Serial.println();
  for(int i = 0; i < 1; i++){
    digitalWrite(led, HIGH);
    delay(350);
    digitalWrite(led, LOW);
    delay(350);
  }
  
}
