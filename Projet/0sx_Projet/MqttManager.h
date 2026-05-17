#pragma once
#include <Arduino.h>
#include <WiFiEspAT.h>
#include <PubSubClient.h>
#include "StateManager.h"
#include "arduino_secrets.h"

// Identifiants MQTT
#define AT_BAUD_RATE 115200
#define DEVICE_NAME "etd9"
#define MQTT_PORT 1883
#define MQTT_USER "etdshawi"
#define MQTT_PASS "shawi123"

const char* const MQTT_SERVER = "216.128.180.194";

// Topics
#define TOPIC_SEND "magasin/9/state"
#define TOPIC_RECEIVE "magasin/9/set"

// Intervalle d'envoi (ms)
#define MQTT_SEND_RATE 10000

typedef void (*TopicHandler)(byte* payload, unsigned int length);

struct TopicFunctionMap {
  const char* topic;
  TopicHandler handler;
};

class MqttManager {
private:
  WiFiClient wifiClient;
  PubSubClient mqttClient;
  StateManager& stateManager;

  static MqttManager* instance;

  void wifiInit();
  void printWifiStatus();
  void printMacAddress(byte mac[]);
  bool reconnect();

  static void mqttEvent(char* topic, byte* payload, unsigned int length);
  static void topicManagement(char* topic, byte* payload, unsigned int length);

public:
  static void handleStatut(byte* payload, unsigned int length);

  MqttManager(StateManager& sm);
  void begin();
  void update();
  void sendState(const String& direction, int vitesse, const String& porte, int clients);
};
