const express = require('express');
const cors = require('cors');
const http = require('http');
const WebSocket = require('ws');
const mqtt = require('mqtt');

// Create Express app
const app = express();
app.use(cors());
app.use(express.json());

// MQTT Broker Configuration
const brokerUrl = 'mqtt://smart-home.local';
const options = {
  username: 'admin',
  password: 'admin',
  clientId: 'nodejs-controller-' + Math.random().toString(16).substr(2, 8)
};

// Connect to MQTT Broker
const mqttClient = mqtt.connect(brokerUrl, options);

mqttClient.on('connect', () => {
  console.log('🔗 Connected to MQTT broker');

  // Subscribe to all device statuses
  mqttClient.subscribe('device/status/#', (err) => {
    if (!err) console.log('📡 Subscribed to device/status/#');
  });
  mqttClient.subscribe('SF/TEMP', (err) => {
    if (!err) console.log("📡 Subscribed to SF/TEMP")
  })
});

mqttClient.on('error', (err) => {
  console.error('❌ MQTT connection error:', err);
});

// Create HTTP server
const server = http.createServer(app);

// WebSocket server
const wss = new WebSocket.Server({ server });

wss.on('connection', (ws) => {
  console.log('🔌 WebSocket client connected');
  ws.send(JSON.stringify({ type: 'info', message: '✅ Connected to WebSocket server' }));

  // Handle incoming WebSocket messages
  ws.on('message', (data) => {
    try {
      const msg = JSON.parse(data.toString());
      console.log("Websocketmsg: ", msg);

      if (!msg.deviceId || !msg.command) {
        console.warn('⚠️ Invalid WS message format:', msg);
        return;
      }

      const topic = `device/led/${msg.deviceId}`;
      const topic2 = `device/lcd/${msg.deviceId}`;
      
      const payload = msg.command.toUpperCase(); // "ON" or "OFF"

      mqttClient.publish(topic, payload, {}, (err) => {
        if (err) {
          console.error('❌ MQTT publish error:', err);
        } else {
          console.log(`📤 Published to MQTT: ${topic} → ${payload}`);
        }
      });
      mqttClient.publish(topic2, payload, {}, (err) => {
        if (err) {
          console.error('❌ MQTT publish error:', err);
        } else {
          console.log(`📤 Published to MQTT: ${topic2} → ${payload}`);
        }
      });
    } catch (e) {
      console.error('❌ Failed to parse WS message:', e.message);
    }
  });
});

// Relay MQTT messages back to WebSocket clients
mqttClient.on('message', (topic, message) => {
  const parts = topic.split('/');
  if (parts.length < 3) return;

  const deviceId = parts[2];
  const payload = message.toString();

  const wsMessage = {
    type: 'status',
    deviceId,
    status: payload === 'ON'
  };

  if (topic === "SF/TEMP"){
    const msg = message.toString();

    const tempMessage = {
      type: 'status',
      device: "temp",
      value: msg
    }


  }


  console.log('🔄 MQTT → WS:', wsMessage);
  console.log('🔄 MQTT → WS:', tempMessage);

  // Send to all connected clients
  wss.clients.forEach((client) => {
    if (client.readyState === WebSocket.OPEN) {
      client.send(JSON.stringify(wsMessage));
      client.send(JSON.stringify(tempMessage));
    }
  });
});

// HTTP test route
app.get('/', (req, res) => {
  res.send('🌐 WebSocket + MQTT server is working!');
});

// Start server
const PORT = 3001;
server.listen(PORT, () => {
  console.log(`🚀 Server running at http://localhost:${PORT}`);
});
