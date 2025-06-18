const { DEVICE_TYPES } = require('../config');
const { mqttClient } = require('../mqtt/client');

module.exports = function setupWebSocket(wss) {
  wss.on('connection', (ws) => {
    console.log('🔌 WebSocket client connected');
    ws.send(JSON.stringify({ type: 'info', message: '✅ Connected to WebSocket server' }));

    ws.on('message', (data) => {
      try {
        const msg = JSON.parse(data);
        const { deviceId, command, deviceType } = msg;

        if (!deviceId || !command || !DEVICE_TYPES.includes(deviceType)) return;

        const topic = `device/${deviceType}/${deviceId}`;
        mqttClient.publish(topic, command.toUpperCase(), {}, (err) => {
          if (err) console.error('❌ MQTT publish error:', err);
          else console.log(`📤 Published to MQTT: ${topic} → ${command.toUpperCase()}`);
        });
      } catch (e) {
        console.error('❌ WS message error:', e.message);
      }
    });
  });
};
