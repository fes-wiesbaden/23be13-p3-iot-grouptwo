#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>

// LCD setup
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Adjust address if needed (commonly 0x27 or 0x3F)

// WiFi credentials
const char* ssid = "FES-SuS";
const char* password = "SuS-WLAN!Key24";

// MQTT credentials
const char* mqtt_server = "10.93.138.100";
const int mqttPort = 1883;
const char* mqttUser = "admin";
const char* mqttPassword = "admin";

// DHT settings
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// MQTT and WiFi clients
WiFiClient espClient;
PubSubClient client(espClient);

// Variables displayed on LCD (manually controlled)
int temp_var = 25;
int Hum_var = 10;

// WiFi connection
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

// MQTT callback
void callback(char* topic, byte* payload, unsigned int length) {
  String message;
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  Serial.println(message);

  if ((String)topic == "device/btn/controlbtns") {
    if (message == "+") {
      temp_var++;
    } else if (message == "-") {
      temp_var--;
    } else if (message == "H+") {
      Hum_var++;
    } else if (message == "H-") {
      Hum_var--;
    }

    // Optional clamping
    temp_var = constrain(temp_var, 0, 50);
    Hum_var = constrain(Hum_var, 0, 100);
  }
}

// MQTT reconnect logic
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "esp32-" + String(random(0xffff), HEX);

    if (client.connect(clientId.c_str(), mqttUser, mqttPassword)) {
      Serial.println("connected");
      client.subscribe("device/btn/controlbtns");
      client.publish("device/btn/controlbtns", "ONLINE");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  lcd.init();
  lcd.clear();         
  lcd.backlight();

  Serial.begin(115200);
  dht.begin();
  setup_wifi();
  client.setServer(mqtt_server, mqttPort);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  if (isnan(temp) || isnan(hum)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println(" °C");

  Serial.print("Humidity: ");
  Serial.print(hum);
  Serial.println(" %");

  // Update LCD display
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temp_var);
  lcd.print((char)223);  // Degree symbol
  lcd.print("C  ");       // Padding for old characters

  lcd.setCursor(0, 1);
  lcd.print("Humi: ");
  lcd.print(Hum_var);
  lcd.print("%   ");      // Padding

  // Publish sensor data to MQTT
  char tempString[8];
  dtostrf(temp, 1, 2, tempString);
  client.publish("SF/TEMP", tempString);

  char humString[8];
  dtostrf(hum, 1, 2, humString);
  client.publish("SF/HUMIDITY", humString);

  delay(500);  // Limit publishing rate
}
