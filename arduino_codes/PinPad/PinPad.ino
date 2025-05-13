#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <Keypad.h>

// KeyPad
const byte ROWS = 4; 
const byte COLS = 4; 

String inputCode = "";

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6}; 
byte colPins[COLS] = {4, 5, 3, 2}; 

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

// Unique MAC address (change last byte per device)
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0x00, 0x02 };
IPAddress fallbackIP(172, 16, 0, 101);

// MQTT configuration
const char* mqttHost = "10.93.138.100";
const int mqttPort = 1883;
const char* mqttUser = "admin";
const char* mqttPassword = "admin";

// Topics
const char* deviceId = "pinpad";
const char* controlTopic = "device/pinpad/pinpad";
const char* statusTopic = "device/pinpad/pinpad";
const char* statusTopicServo = "device/servo/servo";

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

  char customKey = customKeypad.getKey();
  if (customKey){
    inputCode += customKey;

    while (inputCode.length() < 6) {
    char nextKey = customKeypad.getKey();
    if (nextKey) {
      inputCode += nextKey;
    }
    }
    checkCode(inputCode);
    inputCode = "";
  }
}

uint8_t readnumber(void) {
  uint8_t num = 0;
  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}

bool checkCode(const String& code) {
  if (code == "123456") {
    client.publish(statusTopicServo, "ON"); 
    client.publish(statusTopic, "OPEN");
    delay(10000);
    client.publish(statusTopicServo, "OFF"); 
    client.publish(statusTopic, "CLOSE");
    return true;
  } else {
    delay(2000);
    Serial.print("false");
    return false;
  }
}