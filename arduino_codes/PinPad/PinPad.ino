#include <ESP8266WiFi.h>  // For D1 Mini WiFi (ESP8266)
#include <PubSubClient.h> // For MQTT communication

// --- WiFi Credentials ---
const char *ssid = "FES-SuS";
const char *password = "SuS-WLAN!Key24";

// --- MQTT Credentials ---
const char *mqtt_server = "10.93.131.8";
const int mqttPort = 1883;
const char *mqttUser = "admin";
const char *mqttPassword = "admin";
const char *mqtt_client_id = "D1Mini_SmartLock_Client";          // Unique ID for the lock
const char *mqtt_topic_publish_key = "smartlock/keypad/presses"; // Topic for raw key presses
const char *mqtt_topic_publish_status = "smartlock/status";      // Topic for lock status (e.g., "LOCKED", "UNLOCKED")
const char *mqtt_topic_publish_attempt = "smartlock/attempts";   // Topic for code attempts (e.g., "CORRECT", "WRONG", "RESET")

// --- MQTT and WiFi clients ---
WiFiClient espClient;
PubSubClient client(espClient);

// --- Keypad Configuration ---
const int rowPins[] = {D1, D2, D3, D4}; // D1 Mini pins connected to keypad rows
const int colPins[] = {D5, D6, D7};     // D1 Mini pins connected to keypad columns
const int numRows = 4;
const int numCols = 3;

char keys[numRows][numCols] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'} // '*' for Clear, '#' for Enter
};

// --- Debouncing Configuration ---
long lastDebounceTime[numCols];
int debounceDelay = 200;

// --- Smart Lock Specific Configuration ---
const char *CORRECT_CODE = "1234"; // The secret code. '#' is typically used as the enter key.
const int MAX_CODE_LENGTH = 10;    // Max length of code to enter (e.g., 9 digits + '#' )
String enteredCode = "";           // Stores the code entered by the user

bool isLocked = true; // Initial state of the lock

// --- WiFi Connection Function ---
void setup_wifi()
{
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

// --- MQTT Reconnect Logic ---
void reconnect_mqtt()
{
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(mqtt_client_id, mqttUser, mqttPassword))
    {
      Serial.println("connected");
      // Subscribe to a command topic if you want to control the lock remotely
      client.subscribe("smartlock/commands");
      // Publish initial status
      client.publish(mqtt_topic_publish_status, isLocked ? "LOCKED" : "UNLOCKED");
      client.publish(mqtt_topic_publish_attempt, "ONLINE");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

// --- MQTT Callback (for incoming messages, e.g., remote lock/unlock) ---
void callback(char *topic, byte *payload, unsigned int length)
{
  String message;
  for (unsigned int i = 0; i < length; i++)
  {
    message += (char)payload[i];
  }

  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  Serial.println(message);

  // Example for remote commands
  if (String(topic) == "smartlock/commands")
  {
    if (message == "LOCK")
    {
      isLocked = true;
      Serial.println("Lock command received: LOCKED");
      client.publish(mqtt_topic_publish_status, "LOCKED");
      // Optionally activate a "locked" indicator (e.g., red LED)
    }
    else if (message == "UNLOCK")
    {
      // Be careful with remote unlock without authentication!
      isLocked = false;
      Serial.println("Unlock command received: UNLOCKED");
      client.publish(mqtt_topic_publish_status, "UNLOCKED");
      // Optionally activate an "unlocked" indicator (e.g., green LED)
    }
  }
}

void setup()
{
  Serial.begin(115200);

  // --- Keypad Pin Setup ---
  for (int i = 0; i < numRows; i++)
  {
    pinMode(rowPins[i], OUTPUT);
    digitalWrite(rowPins[i], HIGH);
  }
  for (int i = 0; i < numCols; i++)
  {
    pinMode(colPins[i], INPUT_PULLUP);
    lastDebounceTime[i] = 0; // Initialize last debounce time
  }

  // --- WiFi Setup ---
  setup_wifi();

  // --- MQTT Client Setup ---
  client.setServer(mqtt_server, mqttPort);
  client.setCallback(callback);
}

void loop()
{
  // --- Check and Reconnect MQTT ---
  if (!client.connected())
  {
    reconnect_mqtt();
  }
  client.loop(); // Required for PubSubClient to maintain connection and process messages

  // --- Keypad Scan Logic ---
  for (int i = 0; i < numRows; i++)
  {
    digitalWrite(rowPins[i], LOW); // Activate current row

    for (int j = 0; j < numCols; j++)
    {
      if (digitalRead(colPins[j]) == LOW && (millis() - lastDebounceTime[j]) > debounceDelay)
      {
        char pressedKey = keys[i][j];

        Serial.print("Taste gedrückt: ");
        Serial.println(pressedKey);

        // --- Publish raw key press to MQTT ---
        char keyBuffer[2];
        sprintf(keyBuffer, "%c", pressedKey);
        client.publish(mqtt_topic_publish_key, keyBuffer); // Publish each individual key press

        // --- Smart Lock Logic ---
        if (pressedKey == '#')
        { // Enter key
          Serial.print("Code entered: ");
          Serial.println(enteredCode);

          if (enteredCode == CORRECT_CODE)
          {
            Serial.println("Code CORRECT! Lock UNLOCKED.");
            isLocked = false;
            client.publish(mqtt_topic_publish_status, "UNLOCKED");
            client.publish(mqtt_topic_publish_attempt, "CORRECT");
            // Add your physical lock/unlock mechanism here (e.g., relay for a solenoid)
            // Example: digitalWrite(RELAY_PIN, HIGH); // Assuming HIGH unlocks
          }
          else
          {
            Serial.println("Code WRONG! Lock LOCKED.");
            isLocked = true; // Ensure it's locked on wrong attempt
            client.publish(mqtt_topic_publish_status, "LOCKED");
            client.publish(mqtt_topic_publish_attempt, "WRONG");
            // Optionally, implement a lockout delay after multiple wrong attempts
          }
          enteredCode = ""; // Clear the entered code after attempt
        }
        else if (pressedKey == '*')
        { // Clear key
          Serial.println("Code cleared.");
          enteredCode = ""; // Clear the entire entered code
          client.publish(mqtt_topic_publish_attempt, "RESET");
          // Optionally play a clear tone
        }
        else if (enteredCode.length() < MAX_CODE_LENGTH)
        { // Add digit to code
          enteredCode += pressedKey;
          Serial.print("Current code input: ");
          Serial.println(enteredCode);
        }
        else
        {
          Serial.println("Max code length reached. Please press # or *.");
          // Optionally play a warning tone if max length is reached without # or *
        }

        lastDebounceTime[j] = millis(); // Update the last detected time for this column

        // --- Wait for button release ---
        while (digitalRead(colPins[j]) == LOW)
        {
          delay(5);
          client.loop();
        }
      }
    }
    digitalWrite(rowPins[i], HIGH);
  }
}