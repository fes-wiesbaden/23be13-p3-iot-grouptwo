# MQTT lokal auf dem MAC
``
brew install mosquitto
``

``
brew services start mosquitto
``

# MQTT testen
Öffne zwei Terminal
### Subscriber (hört auf Nachrichten):
``
mosquitto_sub -h localhost -t test/topic
``
### Publisher (sendet Nachricht)::
``
mosquitto_pub -h localhost -t test/topic -m "Hello from Mac"
``

Du solltest im Subscriber-Fenster sehen:
``
Hello from Mac"
``

## Mosquitto für WebSocket aktivieren

``
nano /opt/homebrew/etc/mosquitto/mosquitto.conf
``

Füge am Ende hinzu:<br>
listener 9001 <br>
protocol websockets

## MQTT auf Server verwenden
IMPORTANT:<br>
``
mkdir mqtt-test
cd mqtt-test
npm init -y
npm install mqtt
``

## Mosquitto ohne Authentifizierung betreiben
Öffne deine mosquitto.conf, z.B.:
``
nano /opt/homebrew/etc/mosquitto/mosquitto.conf
``

Und füge explizit hinzu:
``
allow_anonymous true
``



