#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

// MAC address (must be unique per device)
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0x00, 0x02 };
IPAddress fallbackIP(172, 16, 0, 101);

// MQTT config
const char* mqttHost = "10.93.138.100";
const int mqttPort = 1883;
const char* mqttUser = "admin";
const char* mqttPassword = "admin";

// Device array
struct Device {
  const char* id;
  int pin;
};

Device leds[] = {
  { "led1", 5 },
  { "led2", 4 }
};


Device btns[] = {
  { "add", 6 },
  { "sub", 7 }
};

// Track previous states
bool prev_add_btn_state = LOW;
bool prev_sub_btn_state = LOW;

unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50; // 50ms debounce


int add_btn_status;
int sub_btn_status;

// Define the pin number for the PIR sensor
const int pirPin = 2;
// Declare and initialize the state variable
int state = 0;

const int ledCount = sizeof(leds) / sizeof(leds[0]);

EthernetClient ethClient;
PubSubClient client(ethClient);

// Helper to build control topic
String controlTopic(const char* id) {
  return "device/lamp/" + String(id);
}

// String btnControl(const char* id){
//   return "device/btn/controlbtns";
// }

// Helper to build status topic
String statusTopic(const char* id) {
  return "device/status/" + String(id);
}

// MQTT callback
void callback(char* topic, byte* payload, unsigned int length) {
  String message;
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  Serial.print("Message on ");
  Serial.print(topic);
  Serial.print(": ");
  Serial.println(message);

  for (int i = 0; i < ledCount; i++) {
    String expectedTopic = controlTopic(leds[i].id);
    if (String(topic) == expectedTopic) {
      bool isOn = message.equalsIgnoreCase("ON");
      digitalWrite(leds[i].pin, isOn ? HIGH : LOW);
      client.publish(statusTopic(leds[i].id).c_str(), isOn ? "ON" : "OFF");
    }
  }
}

// MQTT reconnect logic
void reconnect() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT... ");
    String clientId = "arduino-" + String(random(0xffff), HEX);
    if (client.connect(clientId.c_str(), mqttUser, mqttPassword)) {
      Serial.println("connected!");
      for (int i = 0; i < ledCount; i++) {
        client.subscribe(controlTopic(leds[i].id).c_str());
        client.publish(statusTopic(leds[i].id).c_str(), "ONLINE");
      }
    } else {
      Serial.print("Failed. State=");
      Serial.println(client.state());
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(pirPin, INPUT);  // Set the PIR pin as an input
  for (int i = 0; i < ledCount; i++) {
    pinMode(leds[i].pin, OUTPUT);
    digitalWrite(leds[i].pin, LOW);
  }

  for (int i = 0; i < 2 ; i++){
    pinMode(btns[i].pin, OUTPUT);
  }

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

  state = digitalRead(pirPin);         // Read the state of the PIR sensor
  if (state == HIGH) {                 // If the PIR sensor detects movement (state = HIGH)
    Serial.println("Somebody here!");  // Print "Somebody here!" to the serial monitor
    digitalWrite(4, HIGH);
    digitalWrite(5, LOW);
  } else {
    // Serial.println("Monitoring...");
    digitalWrite(5, HIGH);
    digitalWrite(4, LOW);

    delay(100);
  }

  // Read current button states
bool add_btn_status = digitalRead(btns[0].pin);
bool sub_btn_status = digitalRead(btns[1].pin);

unsigned long currentMillis = millis();

if (currentMillis - lastDebounceTime > debounceDelay) {
  // Detect rising edge (LOW to HIGH)
  if (add_btn_status == HIGH && prev_add_btn_state == LOW) {
    Serial.println("+");
    client.publish("device/btn/controlbtns", "+");
    lastDebounceTime = currentMillis;
  }

  if (sub_btn_status == HIGH && prev_sub_btn_state == LOW) {
    Serial.println("-");
    client.publish("device/btn/controlbtns", "-");
    lastDebounceTime = currentMillis;
  }
}

// Update previous states
prev_add_btn_state = add_btn_status;
prev_sub_btn_state = sub_btn_status;

}
