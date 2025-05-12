#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Servo.h>

// Servo
Servo myservo; 
bool shouldMove = false;

// Unique MAC address (change last byte per device)
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0x00, 0x02 };
IPAddress fallbackIP(172, 16, 0, 101);

const char* ssid = "FES-SuS";
const char* password = "SuS-WLAN!Key24";

// MQTT configuration
const char* mqttHost = "10.93.138.100";
const int mqttPort = 1883;
const char* mqttUser = "admin";
const char* mqttPassword = "admin";

// Topics
const char* deviceId = "servo";
const char* controlTopic = "device/servo/servo";
const char* statusTopic = "device/status/servo";


WiFiClient wifiClient;
PubSubClient client(wifiClient);

// MQTT message handler
void callback(char* topic, byte* payload, unsigned int length) {
  String message;
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  Serial.print("Message on ");
  Serial.print(topic);
  Serial.print(": ");
  Serial.println(message);

  if (String(topic) == controlTopic) {
    if (message == "ON") {
      moveServoOpen();
      client.publish(statusTopic, "ON");
    } else if (message == "OFF") {
      moveServoClose();
      client.publish(statusTopic, "OFF");
    }
  } 
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT... ");
    String clientId = "arduino-" + String(random(0xffff), HEX);
    if (client.connect(clientId.c_str(), mqttUser, mqttPassword)) {
      Serial.println("connected!");
      client.subscribe(controlTopic);
      client.publish(statusTopic, "ONLINE");
    } else {
      Serial.print("Failed. State=");
      Serial.println(client.state());
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  delay(1000);

  myservo.attach(D0);

  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());

  client.setServer(mqttHost, mqttPort);
  client.setCallback(callback);

}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
  Serial.println("WiFi lost! Reconnecting...");
  WiFi.begin(ssid, password);
  }

  if (!client.connected()) {
    reconnect();
  }

  client.loop();

}

void moveServoOpen() {
  myservo.write(0);      // Startposition
  delay(500);
}
void moveServoClose() {
  myservo.write(120);     // Bewege zu 90°
  delay(500);
}