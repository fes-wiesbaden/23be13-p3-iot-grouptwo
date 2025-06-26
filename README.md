# 🏠 Smart Home IoT System

Ein umfassendes IoT Smart Home System mit Arduino/ESP32 Geräten, MQTT-Kommunikation, Node.js Backend und Vue.js Dashboard.


## 📋 Inhaltsverzeichnis

- [🎯 Überblick](#-überblick)
- [📁 Projektstruktur](#-projektstruktur)
- [🏗️ Systemarchitektur](#️-systemarchitektur)
- [🔧 Hardware-Komponenten](#-hardware-komponenten)
- [🚀 Funktionen](#-funktionen)
- [💻 Software-Komponenten](#-software-komponenten)
  - [Backend Server (`backend_server/`)](#backend-server-backend_server)
  - [🌐 Kommunikationsprotokoll](#-kommunikationsprotokoll)
  - [Frontend Dashboard (`dashboard_client/`)](#frontend-dashboard-dashboard_client)
- [🔧 Installation & Einrichtung](#-installation--einrichtung)
- [📱 Gerätespezifische Funktionalität](#-gerätespezifische-funktionalität)
- [🌐 API-Dokumentation](#-api-dokumentation)
- [📡 MQTT-Topics](#-mqtt-topics)
- [🔒 Sicherheitsaspekte](#-sicherheitsaspekte)
- [🛠️ Konfiguration](#️-konfiguration)
- [📊 Dashboard-Features](#-dashboard-features)
- [📊 System-Monitoring](#-system-monitoring)
- [🤝 Mitwirkung](#-mitwirkung)
- [📄 Lizenz](#-lizenz)
- [🔧 Fehlerbehebung](#-fehlerbehebung)
- [📚 Zusätzliche Hinweise](#-zusätzliche-hinweise)


## 🎯 Überblick

Dieses Projekt implementiert ein vollständiges Smart Home System mit folgenden Hauptfunktionen:

- **Smart Lock System** mit Keypad, RFID und Fingerprint-Authentifizierung
- **Kamera-Überwachung** mit ESP32-CAM
- **Umgebungsmonitoring** (Temperatur, Luftfeuchtigkeit, Flammensensor)
- **Automatisierte Beleuchtung** und Servo-Steuerung
- **Real-time Dashboard** mit Vue.js
- **MQTT-basierte Kommunikation** zwischen allen Geräten


## 📁 Projektstruktur

```
├── arduino_codes/           # Arduino/ESP32 Firmware
│   ├── servo_RFID_buzzer_button/  # Hauptzugangskontrollmodul
│   ├── fingerprint/         # Biometrische Authentifizierung
│   ├── lcd_matrix/          # Display und visuelles Feedback
│   ├── cameraWebServer/     # Videoüberwachung
│   ├── mqtt_connection_*/   # MQTT-Kommunikationsmodule
│   ├── temp_sensor.cpp      # Temperatur-/Feuchtigkeitsüberwachung
│   └── library/             # Geteilte Bibliotheken
├── backend_server/          # Node.js MQTT-Broker und API
├── dashboard_client/        # Vue.js Web-Dashboard
└── docs/                    # Dokumentation
```


## 🏗️ Systemarchitektur

```
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│                 │    │                 │    │                 │
│ Arduino/ESP32   │◄──►│  MQTT Broker    │◄──►│ Node.js Backend │
│   Devices       │    │                 │    │                 │
│                 │    │                 │    │                 │
└─────────────────┘    └─────────────────┘    └─────────────────┘
                                                       ▲
                                                       │
                                                       ▼
                                              ┌─────────────────┐
                                              │                 │
                                              │ Vue.js Frontend │
                                              │   Dashboard     │
                                              │                 │
                                              └─────────────────┘
```


## 🔧 Hardware-Komponenten

### Zugangskontrollsystem
- **RFID-Modul (MFRC522)**: Kartenbasierte Authentifizierung
- **Fingerabdrucksensor**: Biometrische Authentifizierung
- **Servo-Motor**: Elektronischer Schließmechanismus
- **Summer**: Audio-Feedback für Zugangsevents
- **Drucktaster**: Manuelle Überschreibung/Klingelfunktion

### Überwachung & Anzeige
- **LCD-Display (16x2 I2C)**: Statusinformationen
- **LED-Matrix (8x8)**: Visuelle Schloss-Status-Anzeigen
- **ESP32-CAM**: Videoüberwachung und -streaming
- **DHT-Sensor**: Temperatur- und Feuchtigkeitsüberwachung
- **PIR-Bewegungsmelder**: Bewegungserkennung
- **Flammensensor**: Feuererkennung
- **Gassensor (MQ-2)**: Gasleck-Erkennung

### Arduino/ESP32 Geräte

| Gerät | Beschreibung | Pins/Anschlüsse |
|-------|--------------|-----------------|
| **ESP32-CAM** | Kamera-Modul für Live-Streaming | AI-Thinker Modell |
| **Arduino Uno** | LED-Steuerung, PIR-Sensor | Ethernet Shield |
| **ESP8266 (D1 Mini)** | Keypad Smart Lock | D1-D7 Pins |
| **ESP32** | Temperatur/Luftfeuchtigkeit | DHT11, LCD I2C |
| **Arduino Fingerprint** | Biometrische Authentifizierung | Serial Communication |
| **RFID/Servo Board** | Zugangskontrolle | MFRC522, Servo, Buzzer |

### Zugangskontrollsystem
- **RFID-Modul (MFRC522)**: Kartenbasierte Authentifizierung
- **Fingerabdrucksensor**: Biometrische Authentifizierung
- **Servo-Motor**: Elektronischer Schließmechanismus
- **Summer**: Audio-Feedback für Zugangsevents
- **Drucktaster**: Manuelle Überschreibung/Klingelfunktion

### Überwachung & Anzeige
- **LCD-Display (16x2 I2C)**: Statusinformationen
- **LED-Matrix (8x8)**: Visuelle Schloss-Status-Anzeigen
- **ESP32-CAM**: Videoüberwachung und -streaming
- **DHT-Sensor**: Temperatur- und Feuchtigkeitsüberwachung
- **PIR-Bewegungsmelder**: Bewegungserkennung
- **Flammensensor**: Feuererkennung
- **Gassensor (MQ-2)**: Gasleck-Erkennung


## 🚀 Funktionen

### Authentifizierungsmethoden
1. **RFID-Kartenzugang**: Kontaktlose Kartenauthentifizierung
2. **Fingerabdruckerkennung**: Biometrische Sicherheit
3. **Manuelle Überschreibung**: Physischer Tasterzugang

### Sicherheitsfeatures
- Multi-Faktor-Authentifizierung
- Zugangsprotokollierung und -überwachung
- Echtzeit-Statusupdates
- Visuelles und akustisches Feedback

### Umweltüberwachung
- Temperatur- und Feuchtigkeitsverfolgung
- Feuererkennung mit sofortigen Alarmen
- Gasleck-Überwachung
- Bewegungserkennung

### Fernsteuerung
- Web-basiertes Dashboard für Systemsteuerung
- Live-Kamera-Streaming
- Echtzeit-Gerätestatus-Überwachung
- Historische Datenvisualisierung


## 💻 Software-Komponenten

### Backend Server (`backend_server/`)


### 🌐 Kommunikationsprotokoll

Das System nutzt **MQTT** für die Gerätekommunikation mit folgender Topic-Struktur:

### Geräte-Topics
- `device/servo/servo` - Schloss-Steuerbefehle
- `device/fingerprint/fingerprint` - Biometrische Authentifizierung
- `device/lcd/lcd` - Display-Steuernachrichten
- `device/buzzer/buzzer` - Audio-Feedback-Steuerung
- `device/bell/bell` - Türklingel-Benachrichtigungen

### Sensor-Topics
- `SF/TEMP` - Temperaturmessungen
- `SF/HUMIDITY` - Feuchtigkeitsmessungen
- `SF/FlameSensore` - Feuererkennungsalarme

### MQTT-Konfiguration
- **Broker**: `10.93.131.8:1883`
- **Zugangsdaten**: admin/admin
- **WLAN-Netzwerk**: FES-SuS

**Technologien:** Node.js, Express, WebSocket, MQTT, PostgreSQL

```javascript
// Hauptfunktionen:
- MQTT Client & Message Handling
- WebSocket Server für Real-time Updates
- REST API für Sensorendaten
- PostgreSQL Datenbankanbindung
- Device Management
```

**Wichtige Dateien:**
- `server.js` - Hauptserver-Datei
- `mqtt/client.js` - MQTT Client-Konfiguration
- `ws/server.js` - WebSocket-Implementierung
- `db/queries.js` - Datenbankabfragen

### Frontend Dashboard (`dashboard_client/`)

**Technologien:** Vue.js 3, TypeScript, Tailwind CSS, Chart.js

```javascript
// Features:
- Real-time Device Dashboard
- Sensor Data Visualization
- Device Control Interface
- Responsive Design
- Dark/Light Theme Toggle
```

**Komponenten:**
- `Dashboard.vue` - Haupt-Dashboard
- `DeviceCard.vue` - Geräte-Steuerung
- `SensorCard.vue` - Sensordaten-Anzeige
- `CameraCard.vue` - Kamera-Feed


### Geräte-Topics
- `device/servo/servo` - Schloss-Steuerbefehle
- `device/fingerprint/fingerprint` - Biometrische Authentifizierung
- `device/lcd/lcd` - Display-Steuernachrichten
- `device/buzzer/buzzer` - Audio-Feedback-Steuerung
- `device/bell/bell` - Türklingel-Benachrichtigungen

### Sensor-Topics
- `SF/TEMP` - Temperaturmessungen
- `SF/HUMIDITY` - Feuchtigkeitsmessungen
- `SF/FlameSensore` - Feuererkennungsalarme

### MQTT-Konfiguration
- **Broker**: `10.93.131.8:1883`
- **Zugangsdaten**: admin/admin
- **WLAN-Netzwerk**: FES-SuS


## 🔧 Installation & Einrichtung

### Hardware-Setup
1. Verbinden Sie alle Sensoren und Aktoren entsprechend der Pin-Konfiguration in [`arduino_codes/servo_RFID_buzzer_button/servo_RFID_buzzer_button.ino`](arduino_codes/servo_RFID_buzzer_button/servo_RFID_buzzer_button.ino)
2. Flashen Sie die entsprechende Firmware auf jedes Arduino/ESP32-Gerät
3. Konfigurieren Sie die WLAN-Zugangsdaten in jedem Gerätecode

### Backend-Server
```bash
cd backend_server
npm install
npm start
```

### Dashboard-Client
```bash
cd dashboard_client
npm install
npm run dev
```

## 📱 Gerätespezifische Funktionalität

### Hauptsteuereinheit ([`servo_RFID_buzzer_button.ino`](arduino_codes/servo_RFID_buzzer_button/servo_RFID_buzzer_button.ino))
- RFID-Kartenlesen und -authentifizierung
- Servo-Motor-Steuerung für Schließmechanismus
- Summer-Feedback für Zugangsevents
- MQTT-Kommunikation für Systemkoordination

### Fingerabdruckmodul ([`fingerprint.ino`](arduino_codes/fingerprint/fingerprint.ino))
- Biometrisches Fingerabdruck-Scannen
- Benutzeranmeldung und -verifizierung
- Integration mit Hauptzugangskontrollsystem

### Anzeigesystem ([`lcd_matrix.ino`](arduino_codes/lcd_matrix/lcd_matrix.ino))
- LCD-Textanzeige für Systemstatus
- LED-Matrix-Grafiken für Schloss-Status-Visualisierung
- Echtzeit-Statusupdates via MQTT

### Kamerasystem ([`cameraWebServer/`](arduino_codes/cameraWebServer/))
- Live-Video-Streaming
- Web-basierte Kamerasteuerung
- LED-Beleuchtungssteuerung
- HTTP-API für Kameraeinstellungen

### Umweltüberwachung ([`mqtt_connection_esp32.ino`](arduino_codes/mqtt_connection_esp32/mqtt_connection_esp32.ino))
- DHT22 Temperatur-/Feuchtigkeitssensor
- Flammenerkennungssensor
- Gassensor-Überwachung
- Bluetooth-Konnektivitätsunterstützung


## 🌐 API-Dokumentation

### REST Endpoints

```http
GET /                    # Server Status
GET /devices            # Unterstützte Gerätetypen
GET /sensors            # Sensordaten abrufen
```

### WebSocket Events

```javascript
// Client → Server
{
  "deviceId": "led1",
  "command": "ON",
  "deviceType": "led"
}

// Server → Client
{
  "type": "status",
  "device": "temp",
  "value": "23.5"
}
```

## 📡 MQTT-Topics

### Device Control Topics

| Topic | Beschreibung | Payload |
|-------|--------------|---------|
| `device/led/{id}` | LED-Steuerung | `ON/OFF` |
| `device/servo/{id}` | Servo-Steuerung | `ON/OFF` |
| `device/buzzer/{id}` | Buzzer-Steuerung | `BuzzerON/BuzzerOFF` |
| `device/lcd/{id}` | LCD-Steuerung | `accessAllowed/accessDenied` |

### Sensor Data Topics

| Topic | Beschreibung | Datenformat |
|-------|--------------|-------------|
| `SF/TEMP` | Temperaturwerte | Float (°C) |
| `SF/HUMIDITY` | Luftfeuchtigkeit | Float (%) |
| `SF/FlameSensore` | Flammensensor | Boolean |
| `smartlock/status` | Smart Lock Status | `LOCKED/UNLOCKED` |
| `smartlock/keypad/presses` | Keypad-Eingaben | Character |


## 🔒 Sicherheitsaspekte

- MQTT-Broker-Authentifizierung erforderlich
- Verschlüsselte WLAN-Kommunikation
- Zugangsprotokollierung
- Multi-Faktor-Authentifizierungsunterstützung
- Echtzeit-Überwachung und Alarme

## 🛠️ Konfiguration

### Netzwerkeinstellungen
WLAN-Zugangsdaten in jedem Arduino-Sketch aktualisieren:
```cpp
const char* ssid = "FES-SuS";
const char* password = "SuS-WLAN!Key24";
```

### MQTT-Einstellungen
MQTT-Broker-Details konfigurieren:
```cpp
const char* mqttHost = "10.93.131.8";
const int mqttPort = 1883;
const char* mqttUser = "admin";
const char* mqttPassword = "admin";
```

## 📊 Dashboard-Features

Das Web-Dashboard ([`dashboard_client/`](dashboard_client/)) bietet:
- Echtzeit-Gerätestatus-Überwachung
- Umweltdaten-Visualisierung
- Sicherheitsereignis-Protokollierung
- Ferngerätsteuerung
- Kamera-Feed-Integration

## 📊 System-Monitoring

Das System bietet umfassendes Monitoring:

- **Device Status Tracking** - Online/Offline Status aller Geräte
- **Sensor Data Logging** - Historische Datenerfassung
- **Error Handling & Alerts** - Automatische Fehlererkennung
- **Performance Metrics** - Latenz und Durchsatz-Überwachung


## 🤝 Mitwirkung

Dies ist ein Schulprojekt für IoT Gruppe Zwei. Für Beiträge:
1. Folgen Sie der bestehenden Code-Struktur
2. Aktualisieren Sie Gerätekonfigurationen nach Bedarf
3. Testen Sie alle MQTT-Kommunikationspfade
4. Dokumentieren Sie alle Hardware-Änderungen

### 👥 Team

- **Gruppe 2** - IoT Smart Home System
- **Mitglieder:** Sebastian Weiske, Georg Triller, Laurin Schubert, Mahdi Haidary, Maurice-Pascal Ess
- **Institution:** FES


## 📄 Lizenz

Dieses Projekt wurde für Bildungszwecke als Teil des IoT-Lehrplans entwickelt.

## 🔧 Fehlerbehebung

### Häufige Probleme
1. **MQTT-Verbindung fehlgeschlagen**: Broker-IP und Zugangsdaten überprüfen
2. **WLAN-Verbindungsprobleme**: Netzwerk-Zugangsdaten und Signalstärke prüfen
3. **Sensor-Lesefehler**: Verkabelung und Stromversorgung überprüfen
4. **Kamera-Stream funktioniert nicht**: ESP32-CAM Stromversorgung und Netzwerkverbindung prüfen

### Häufige Probleme

**MQTT Connection Failed:**
```bash
# Broker-Status prüfen
mosquitto_pub -h 10.93.131.8 -t test -m "hello"

# Port-Verfügbarkeit testen
telnet 10.93.131.8 1883
```

**Arduino Upload Errors:**
- Richtigen Board-Typ auswählen
- USB-Treiber installieren
- Reset-Button während Upload drücken

**Frontend Build Errors:**
```bash
# Dependencies aktualisieren
npm update
npm audit fix
```


### Debug-Informationen
Aktivieren Sie das serielle Debugging in der Arduino IDE (115200 Baud), um Gerätekommunikation und -status zu überwachen.

## 📚 Zusätzliche Hinweise

### Code-Standards

- **Arduino:** Consistent naming, proper commenting
- **JavaScript:** ESLint + Prettier
- **Vue.js:** Composition API, TypeScript
- **CSS:** Tailwind CSS classes

### Sensorkalibrierung
- **DHT22**: Kalibrierung für lokale Umgebungsbedingungen
- **Flammensensor**: Empfindlichkeitseinstellung je nach Umgebung
- **Gassensor**: Aufwärmzeit von 24-48 Stunden beachten

### Systemwartung
- Regelmäßige RFID-Karten-Reinigung
- Fingerabdrucksensor-Kalibrierung
- MQTT-Verbindungsstabilität überwachen
- Backup der Gerätekonfigurationen

### Erweiterungsmöglichkeiten
- Integration zusätzlicher Sensoren
- Mobile App-Entwicklung
- Cloud-Anbindung für externe Überwachung
- Machine Learning für Verhaltensmuster-Erkennung