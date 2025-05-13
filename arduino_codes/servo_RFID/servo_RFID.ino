#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Buzzer.h>

#define RST_PIN         D0           // Configurable, see typical pin layout above
#define SS_PIN          D8          // Configurable, see typical pin layoutabove

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

MFRC522::MIFARE_Key key;

// Buzzer
Buzzer buzzer(D2);

// Servo
Servo myservo; 
bool shouldMove = false;
unsigned long startZeit = 0;
bool ersteFunktionAusgeführt = false;
bool zweiteFunktionAusgeführt = false;

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
const char* deviceId2 = "buzzer";
const char* controlTopic = "device/servo/servo";
const char* statusTopic = "device/status/servo";
const char* controlTopic2 = "device/buzzer/buzzer";
const char* statusTopic2 = "device/buzzer/buzzer";

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
  if (String(topic) == controlTopic2) {
    if (message == "BuzzerON") {
      buzzerAllowed();
      client.publish(statusTopic, "BuzzerON");
    } else if (message == "BuzzerOFF") {
      buzzerDenied();
      client.publish(statusTopic, "BuzzerOFF");
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
      client.subscribe(controlTopic2);
      client.publish(statusTopic, "ONLINE");
      client.publish(statusTopic2, "ONLINE");
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

  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();        // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522 card

  // Prepare the key (used both as key A and as key B)
  // using FFFFFFFFFFFFh which is the default at chip delivery from the factory
  for (byte i = 0; i < 6; i++) {
      key.keyByte[i] = 0xFF;
  }

  Serial.println(F("Scan a MIFARE Classic PICC to demonstrate read and write."));
  Serial.print(F("Using key (for A and B):"));
  dump_byte_array(key.keyByte, MFRC522::MF_KEY_SIZE);
  Serial.println();

  Serial.println(F("BEWARE: Data will be written to the PICC, in sector #1"));

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
  if (ersteFunktionAusgeführt && !zweiteFunktionAusgeführt) {
    if (millis() - startZeit >= 10000) { // 10 Sekunden später
      client.publish(controlTopic, "OFF"); 
      zweiteFunktionAusgeführt = true;

      ersteFunktionAusgeführt = false;
      zweiteFunktionAusgeführt = false;
    }
  }
  if (WiFi.status() != WL_CONNECTED) {
  Serial.println("WiFi lost! Reconnecting...");
  WiFi.begin(ssid, password);
  }

  if (!client.connected()) {
    reconnect();
  }

  client.loop();

  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! mfrc522.PICC_IsNewCardPresent())
    return;

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial())
    return;

  // Show some details of the PICC (that is: the tag/card)
  Serial.print(F("Card UID:"));
  dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
  Serial.println();
  // Überprüfe auf erlaubte UID (Beispiel: 42 82 B0 39)
  if (mfrc522.uid.size == 4 &&
    mfrc522.uid.uidByte[0] == 0x42 &&
    mfrc522.uid.uidByte[1] == 0x82 &&
    mfrc522.uid.uidByte[2] == 0xB0 &&
    mfrc522.uid.uidByte[3] == 0x39) {
    Serial.println("✅ Zugang gewährt");

  if (!ersteFunktionAusgeführt) {
    client.publish(controlTopic2, "BuzzerON"); 
    client.publish(controlTopic, "ON"); 
    startZeit = millis();  // Startzeit merken
    ersteFunktionAusgeführt = true;
    zweiteFunktionAusgeführt = false;
  }
  } else {
    client.publish(controlTopic2, "BuzzerOFF"); 
    Serial.println("❌ Zugang verweigert");
    return; // Rest des Codes wird nicht ausgeführt
  }
  Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  Serial.println(mfrc522.PICC_GetTypeName(piccType));

  // Check for compatibility
  if (    piccType != MFRC522::PICC_TYPE_MIFARE_MINI
    &&  piccType != MFRC522::PICC_TYPE_MIFARE_1K
    &&  piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("This sample only works with MIFARE Classic cards."));
    return;
  }

  // In this sample we use the second sector,
  // that is: sector #1, covering block #4 up to and including block #7
  byte sector         = 1;
  byte blockAddr      = 4;
  byte dataBlock[]    = {
    0x01, 0x02, 0x03, 0x04, //  1,  2,   3,  4,
    0x05, 0x06, 0x07, 0x08, //  5,  6,   7,  8,
    0x09, 0x0a, 0xff, 0x0b, //  9, 10, 255, 11,
    0x0c, 0x0d, 0x0e, 0x0f  // 12, 13, 14, 15
  };
  byte trailerBlock   = 7;
  MFRC522::StatusCode status;
  byte buffer[18];
  byte size = sizeof(buffer);

  // Authenticate using key A
  Serial.println(F("Authenticating using key A..."));
  status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  // Show the whole sector as it currently is
  Serial.println(F("Current data in sector:"));
  mfrc522.PICC_DumpMifareClassicSectorToSerial(&(mfrc522.uid), &key, sector);
  Serial.println();

  // Read data from the block
  Serial.print(F("Reading data from block ")); Serial.print(blockAddr);
  Serial.println(F(" ..."));
  status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(blockAddr, buffer, &size);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("MIFARE_Read() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
  }
  Serial.print(F("Data in block ")); Serial.print(blockAddr); Serial.println(F(":"));
  dump_byte_array(buffer, 16); Serial.println();
  Serial.println();

  // Authenticate using key B
  Serial.println(F("Authenticating again using key B..."));
  status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, trailerBlock, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  // Write data to the block
  Serial.print(F("Writing data into block ")); Serial.print(blockAddr);
  Serial.println(F(" ..."));
  dump_byte_array(dataBlock, 16); Serial.println();
  status = (MFRC522::StatusCode) mfrc522.MIFARE_Write(blockAddr, dataBlock, 16);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("MIFARE_Write() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
  }
  Serial.println();

  // Read data from the block (again, should now be what we have written)
  Serial.print(F("Reading data from block ")); Serial.print(blockAddr);
  Serial.println(F(" ..."));
  status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(blockAddr, buffer, &size);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("MIFARE_Read() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
  }
  Serial.print(F("Data in block ")); Serial.print(blockAddr); Serial.println(F(":"));
  dump_byte_array(buffer, 16); Serial.println();

  // Check that data in block is what we have written
  // by counting the number of bytes that are equal
  Serial.println(F("Checking result..."));
  byte count = 0;
  for (byte i = 0; i < 16; i++) {
    // Compare buffer (= what we've read) with dataBlock (= what we've written)
    if (buffer[i] == dataBlock[i])
      count++;
  }
  Serial.print(F("Number of bytes that match = ")); Serial.println(count);
  if (count == 16) {
    Serial.println(F("Success :-)"));
  } else {
    Serial.println(F("Failure, no match :-("));
    Serial.println(F("  perhaps the write didn't work properly..."));
  }
  Serial.println();

  // Dump the sector data
  Serial.println(F("Current data in sector:"));
  mfrc522.PICC_DumpMifareClassicSectorToSerial(&(mfrc522.uid), &key, sector);
  Serial.println();

  // Halt PICC
  mfrc522.PICC_HaltA();
  // Stop encryption on PCD
  mfrc522.PCD_StopCrypto1();
}

void moveServoOpen() {
  myservo.write(0);      // Startposition
  delay(500);
}
void moveServoClose() {
  myservo.write(120);     // Bewege zu 90°
  delay(500);
}

void dump_byte_array(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

void buzzerAllowed() {
  buzzer.begin(10);
  buzzer.sound(NOTE_C5, 100);
  buzzer.sound(NOTE_E5, 100);
  buzzer.sound(NOTE_G5, 200);
  buzzer.sound(0, 100);       
  buzzer.sound(NOTE_G5, 300);     
  buzzer.end(1000);
}

void buzzerDenied() {
  buzzer.begin(10);
  buzzer.sound(NOTE_G4, 200);
  buzzer.sound(NOTE_E4, 200);
  buzzer.sound(NOTE_C4, 300);
  buzzer.sound(0, 100);           
  buzzer.end(1000);
  delay(2000);
}