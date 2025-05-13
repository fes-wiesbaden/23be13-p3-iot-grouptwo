#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

// Unique MAC address (change last byte per device)
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0x00, 0x02 };
IPAddress fallbackIP(172, 16, 0, 101);

// MQTT configuration
const char* mqttHost = "10.93.138.100";
const int mqttPort = 1883;
const char* mqttUser = "admin";
const char* mqttPassword = "admin";

// Topics
const char* deviceId = "led1";
const char* controlTopic1 = "device/led/led1";
const char* controlTopic2 = "device/led/led2";
const char* statusTopic1 = "device/status/led1";
const char* statusTopic2 = "device/status/led2";

// Pins
const int LED1 = 5;
const int LED2 = 4;

EthernetClient ethClient;
PubSubClient client(ethClient);

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

  if (String(topic) == controlTopic1) {
    if (message == "ON") {
      digitalWrite(LED1, HIGH);
      client.publish(statusTopic1, "ON");
    } else if (message == "OFF") {
      digitalWrite(LED1, LOW);
      client.publish(statusTopic1, "OFF");
    }
  } else if (String(topic) == controlTopic2) {
    if (message == "ON") {
      digitalWrite(LED2, HIGH);
      client.publish(statusTopic2, "ON");
    } else if (message == "OFF") {
      digitalWrite(LED2, LOW);
      client.publish(statusTopic2, "OFF");
    }
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT... ");
    String clientId = "arduino-" + String(random(0xffff), HEX);
    if (client.connect(clientId.c_str(), mqttUser, mqttPassword)) {
      Serial.println("connected!");
      client.subscribe(controlTopic1);
      client.subscribe(controlTopic2);
      client.publish(statusTopic1, "ONLINE");
      client.publish(statusTopic2, "ONLINE");
    } else {
      Serial.print("Failed. State=");
      Serial.println(client.state());
      delay(5000);
    }
  }
}

void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);

  Serial.begin(9600);
  delay(1000);

  Serial.println("Starting Ethernet...");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("DHCP failed, using fallback IP.");
    Ethernet.begin(mac, fallbackIP);
  }

  delay(1500);
  Serial.print("IP: ");
  Serial.println(Ethernet.localIP());

  client.setServer(mqttHost, mqttPort);
  client.setCallback(callback);
}

void loop() {
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("⚠️ Ethernet cable disconnected!");
  }

  if (!client.connected()) {
    reconnect();
  }

  client.loop();
}
