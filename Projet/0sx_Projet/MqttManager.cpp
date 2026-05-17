#include "MqttManager.h"

// Instance statique pour le callback
MqttManager* MqttManager::instance = nullptr;

// Mapping des topics reçus
TopicFunctionMap handlers[] = {
  { "set", MqttManager::handleStatut }
};
const int handlersSize = sizeof(handlers) / sizeof(handlers[0]);

// Constructeur
MqttManager::MqttManager(StateManager& sm)
  : mqttClient(wifiClient), stateManager(sm) {
  instance = this;
}

// --Initialisation
void MqttManager::begin() {
  wifiInit();

  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
  mqttClient.setCallback(mqttEvent);

  if (!mqttClient.connect(DEVICE_NAME, MQTT_USER, MQTT_PASS)) {
    Serial.println("Incapable de se connecter sur le serveur MQTT");
    Serial.print("client.state : ");
    Serial.println(mqttClient.state());
  } else {
    Serial.println("Connecte sur le serveur MQTT");
    mqttClient.subscribe(TOPIC_RECEIVE, 0);
  }

  Serial.println("Setup complete");
}

// --Mise à jour (à appeler dans loop)
void MqttManager::update() {
  if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.loop();
}

// Envoi des données du magasin
// Valeurs direction : "gauche", "droite", "stop"
// Valeurs porte     : "move", "ouvert", "ferme"
// Valeurs statut    : "rabais", "ok" (normal/ferme), "urgence", "erreur"
void MqttManager::sendState(const String& direction, int vitesse, const String& porte, int clients) {
  static unsigned long lastTime = 0;

  if (millis() - lastTime < MQTT_SEND_RATE) return;
  lastTime = millis();

  String statut = stateManager.getStatutLabel();

  static char message[150];
  sprintf(message,
          "{\"direction\":\"%s\","
          "\"vitesse\":%d,"
          "\"porte\":\"%s\","
          "\"statut\":\"%s\","
          "\"clients\":%d}",
          direction.c_str(),
          vitesse,
          porte.c_str(),
          statut.c_str(),
          clients);

  Serial.print("Envoie : ");
  Serial.println(message);

  if (!mqttClient.publish(TOPIC_SEND, message)) {
    reconnect();
    Serial.println("Incapable d'envoyer le message!");
  } else {
    Serial.println("Message envoye");
  }
}

// Reconnexion MQTT
bool MqttManager::reconnect() {
  static unsigned long lastAttempt = 0;
  if (millis() - lastAttempt < 5000) return false;
  lastAttempt = millis();

  bool result = mqttClient.connect(DEVICE_NAME, MQTT_USER, MQTT_PASS);
  if (!result) {
    Serial.println("Incapable de se connecter sur le serveur MQTT");
  } else {
    mqttClient.subscribe(TOPIC_RECEIVE, 0);
    Serial.println("Reconnecte au serveur MQTT");
  }
  return result;
}

// Callback MQTT (statique)
void MqttManager::mqttEvent(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message recu [");
  Serial.print(topic);
  Serial.print("] ");
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  char* specificTopic = topic + strlen("magasin/9/");
  topicManagement(specificTopic, payload, length);
}

// Dispatch des topics reçus
void MqttManager::topicManagement(char* topic, byte* payload, unsigned int length) {
  for (int i = 0; i < handlersSize; i++) {
    if (strcmp(topic, handlers[i].topic) == 0) {
      handlers[i].handler(payload, length);
      return;
    }
  }
  Serial.print("Topic non gere : ");
  Serial.println(topic);
}

// Handler : réception du statut magasin
void MqttManager::handleStatut(byte* payload, unsigned int length) {
  if (instance == nullptr) return;

  char buf[50];
  unsigned int copyLen = length < sizeof(buf) - 1 ? length : sizeof(buf) - 1;
  memcpy(buf, payload, copyLen);
  buf[copyLen] = '\0';

  if (strstr(buf, "rabais")) instance->stateManager.setState(DISCOUNT);
  else if (strstr(buf, "ok")) instance->stateManager.setState(NORMAL);
  else if (strstr(buf, "erreur")) instance->stateManager.setState(ERROR);
  else if (strstr(buf, "urgence")) instance->stateManager.setState(EMERGENCY_STORE);
  else {
    Serial.print("Statut inconnu recu : ");
    Serial.println(buf);
  }
}

// Initialisation WiFi
// Pour changer de réseau, modifier SECRET_SSID et SECRET_PASS dans arduino_secrets.h
void MqttManager::wifiInit() {
  Serial1.begin(AT_BAUD_RATE);
  WiFi.init(Serial1);

  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("La communication avec le module WiFi a echoue!");
    while (true) {
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    }
  }

  WiFi.begin(SECRET_SSID, SECRET_PASS);

  Serial.println("En attente de connexion au WiFi");

  unsigned long lastPrint = 0;
  while (WiFi.status() != WL_CONNECTED) {
    if (millis() - lastPrint >= 1000) {
      lastPrint = millis();
      Serial.print('.');
    }
  }

  Serial.println();
  Serial.println("Connecte au reseau WiFi.");
  printWifiStatus();
}

// Affichage du statut WiFi
void MqttManager::printWifiStatus() {
  char ssid[33];
  WiFi.SSID(ssid);
  Serial.print("SSID: ");
  Serial.println(ssid);

  uint8_t bssid[6];
  WiFi.BSSID(bssid);
  Serial.print("BSSID: ");
  printMacAddress(bssid);

  uint8_t mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC: ");
  printMacAddress(mac);

  IPAddress ip = WiFi.localIP();
  Serial.print("Adresse IP: ");
  Serial.println(ip);

  long rssi = WiFi.RSSI();
  Serial.print("Force du signal (RSSI): ");
  Serial.print(rssi);
  Serial.println(" dBm");
}

// Affichage de l'adresse MAC
void MqttManager::printMacAddress(byte mac[]) {
  for (int i = 5; i >= 0; i--) {
    if (mac[i] < 16) Serial.print("0");
    Serial.print(mac[i], HEX);
    if (i > 0) Serial.print(":");
  }
  Serial.println();
}
