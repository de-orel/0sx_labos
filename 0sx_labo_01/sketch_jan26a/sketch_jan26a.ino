const int led = 13;
const long etd = 2449427;

void setup() {
  // put your setup code here, to run once:
  pinMode(led, OUTPUT);
  Serial.begin(9600);
}

void loop() {

  allumeEteint();
  variationIntensite();
  clignotement();
}

// État allumé et éteint

void allumeEteint(){

  Serial.print("Etat: Allume - ");
  Serial.println(etd);
  //delay(1000);
  digitalWrite(led, LOW);
  delay(300);
  digitalWrite(led, HIGH);
  delay(2000);
  digitalWrite(led, LOW);
  delay(1000);
}

// État variation d'intensité

void variationIntensite(){

  Serial.print("Etat: Varie - ");
  Serial.println(etd);
  //delay(1000);
  for(int i = 255; i >= 0; i--){      // etd = 2449427;
    analogWrite(led, i);
    delay(8);     // de 0 à 255 on a 256 valeurs, donc on a (2048 / 256) = 8;
  }
}

// État clignotement

void clignotement(){

  int nbClignotements = 1;      // PLAFOND.MATH(2/2) = 1

  Serial.print("Etat: Clignotement - ");
  Serial.println(etd);
  //delay(1000);
  for(int i = 0; i < nbClignotements; i++){      
    digitalWrite(led, HIGH);
    delay(350);
    digitalWrite(led, LOW);
    delay(350);
  }
}

