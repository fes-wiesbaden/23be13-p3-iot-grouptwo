#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

// Matrix Screen

// NOTE: These pin numbers will probably not work with your hardware and may
// need to be adapted
#define HARDWARE_TYPE MD_MAX72XX::PAROLA_HW
#define MAX_DEVICES 2

#define CLK_PIN   18  // or SCK
#define DATA_PIN  23  // or MOSI
#define CS_PIN    5  // or SS

// SPI hardware interface
MD_MAX72XX M = MD_MAX72XX(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

byte openLock[8] = {
  B00111111,
  B01001001,
  B10001001,
  B10001001,
  B10001001,
  B10001001,
  B00001001,
  B00001111
};

byte closeLock[8] = {
  B00111111,
  B01001001,
  B10001001,
  B10001001,
  B10001001,
  B10001001,
  B01001001,
  B00111111
};
 
//LCD Screen
LiquidCrystal_I2C lcd(0x27,16,2); // set the LCD address to 0x3F for a 16 chars and 2 line display
 
// Unique MAC address (change last byte per device)
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0x00, 0x02 };
IPAddress fallbackIP(172, 16, 0, 101);

const char* ssid = "FES-SuS";
const char* password = "SuS-WLAN!Key24";

// MQTT configuration
const char* mqttHost = "10.93.131.8";
const int mqttPort = 1883;
const char* mqttUser = "admin";
const char* mqttPassword = "admin";

// Topics
const char* deviceId = "lcd";
const char* controlTopic = "device/lcd/lcd";
const char* statusTopic = "device/lcd/lcd";

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
    if (message == "accessAllowed") {
      Serial.print("access Allowed");
      accessAllowedTxt();
      drawOpenClock();
      client.publish(statusTopic, "allowed");
    }
    if (message == "accessDenied") {
      accessDeniedTxt();
      drawCloseClock();
      client.publish(statusTopic, "denied");
    }
    if (message == "accessPending") {
      accessPendingTxt();
      drawCloseClock();
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

void setup()
{
  Serial.begin(115200);
  M.begin();
  M.clear();
  lcd.init(); // initialize the lcd
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0,0); 
  lcd.print("Smart Lock");
  lcd.setCursor(0,1);
  lcd.print("System");
  drawCloseClock();

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
 
void loop()
{
  if (WiFi.status() != WL_CONNECTED) {
  Serial.println("WiFi lost! Reconnecting...");
  WiFi.begin(ssid, password);
  }

  if (!client.connected()) {
    reconnect();
  }

  client.loop();
}

void accessAllowedTxt() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Access allowed");
}

void accessDeniedTxt() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Access denied");
}

void accessPendingTxt() {
  lcd.clear();
  lcd.setCursor(0,0); 
  lcd.print("Smart Lock");
  lcd.setCursor(0,1);
  lcd.print("System");
}

void drawCloseClock() {
  drawImage(closeLock);
}

void drawOpenClock() {
  drawImage(openLock);
}

void drawImage(byte* image) {
  for (uint8_t row = 0; row < 8; row++) {
    M.setRow(row, image[row]);
  }
}