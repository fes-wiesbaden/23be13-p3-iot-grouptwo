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

// Define supported device types
const DEVICE_TYPES = ['led', 'lcd', 'servo', 'fan', 'buzzer', 'lamp'];

// Connect to MQTT Broker
const mqttClient = mqtt.connect(brokerUrl, options);

mqttClient.on('connect', () => {
  console.log('🔗 Connected to MQTT broker');

  // Subscribe to device status topics
  DEVICE_TYPES.forEach(type => {
    mqttClient.subscribe(`device/${type}/#`, (err) => {
      if (!err) console.log(`📡 Subscribed to device/${type}/#`);
    });
  });

  // Subscribe to temperature sensor
  mqttClient.subscribe('SF/TEMP', (err) => {
    if (!err) console.log("📡 Subscribed to SF/TEMP");
  });

  // Subscribe to humidity sensor
  mqttClient.subscribe('SF/HUMIDITY', (err) => {
    if (!err) console.log('📡 Subscribed to SF/HUMIDITY');
  });

  mqttClient.subscribe('device/pinpad/pinpad', (err) => {
    if (!err)  console.log('📡 Subscribed to device/pinpad/pinpad')
  })

  mqttClient.subscribe('device/buzzer/buzzer', (err) => {
    if (!err)  console.log('📡 Subscribed to device/buzzer/buzzer')
  })

  mqttClient.subscribe('device/bell/bell', (err) => {
    if (!err)  console.log('📡 Subscribed to device/buzzer/buzzer')
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

  ws.on('message', (data) => {
    try {
      const msg = JSON.parse(data.toString());
      const { deviceId, command, deviceType } = msg;

      if (!deviceId || !command || !deviceType || !DEVICE_TYPES.includes(deviceType)) {
        console.warn('⚠️ Invalid WS message format or unknown device type:', msg);
        return;
      }

      const topic = `device/${deviceType}/${deviceId}`;
      const payload = command.toUpperCase();

      mqttClient.publish(topic, payload, {}, (err) => {
        if (err) {
          console.error('❌ MQTT publish error:', err);
        } else {
          console.log(`📤 Published to MQTT: ${topic} → ${payload}`);
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

  console.log(parts)

  // Handle Temperature update
  if (topic === "SF/TEMP") {
    const tempMsg = {
      type: 'status',
      device: "temp",
      value: message.toString()
    };

    // console.log('🌡️ Temp Update → WS:', tempMsg);

    wss.clients.forEach((client) => {
      if (client.readyState === WebSocket.OPEN) {
        client.send(JSON.stringify(tempMsg));
      }
    });

    return;
  }

  // Handle Humidity update
  if (topic === "SF/HUMIDITY") {
    const humMsg = {
      type: 'status',
      device: "humidity",
      value: message.toString()
    };

    // console.log('💧 Humidity Update → WS:', humMsg);

    wss.clients.forEach((client) => {
      if (client.readyState === WebSocket.OPEN) {
        client.send(JSON.stringify(humMsg));
      }
    });

    return;
  }

  // Handle Door Opener
  if (topic === "device/pinpad/pinpad") {
    const doorMsg = {
      type: 'status',
      device: "pinpad",
      value: message.toString()
    };

    console.log('💧 Door Update → WS:', doorMsg);

    wss.clients.forEach((client) => {
      if (client.readyState === WebSocket.OPEN) {
        client.send(JSON.stringify(doorMsg));
      }
    });

    return;
  }

  // Handle Door Bell
  if (topic === "device/bell/bell") {
    const bellMsg = {
      type: 'status',
      device: "bell",
      value: message.toString()
    };

    console.log('💧 Bell Update → WS:', bellMsg);

    wss.clients.forEach((client) => {
      if (client.readyState === WebSocket.OPEN) {
        client.send(JSON.stringify(bellMsg));
      }
    });

    return;
  }
  // Handle LCD
  if (topic === "device/lcd/lcd") {
    const lcdMsg = {
      type: 'status',
      device: "lcd",
      value: message.toString()
    };

    console.log('💧 Bell Update → WS:', lcdMsg);

    wss.clients.forEach((client) => {
      if (client.readyState === WebSocket.OPEN) {
        client.send(JSON.stringify(lcdMsg));
      }
    });

    return;
  }

  // Handle device status updates
  if (parts.length === 3 && parts[0] === 'device') {
    const [, deviceType, deviceId] = parts;
    const statusMsg = {
      type: 'status',
      deviceType,
      deviceId,
      value: message.toString()
    };

    console.log('🔄 MQTT → WS:', statusMsg);

    wss.clients.forEach((client) => {
      if (client.readyState === WebSocket.OPEN) {
        client.send(JSON.stringify(statusMsg));
      }
    });
  }
});

// HTTP route to test server
app.get('/', (req, res) => {
  res.send('🌐 WebSocket + MQTT server is working!');
});

// Endpoint to get supported device types
app.get('/devices', (req, res) => {
  res.json({ supportedTypes: DEVICE_TYPES });
});

// Start server
const PORT = 3001;
server.listen(PORT, () => {
  console.log(`🚀 Server running at http://localhost:${PORT}`);
});
