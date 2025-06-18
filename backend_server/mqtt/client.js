const mqtt = require('mqtt');
const { mqttOptions, brokerUrl } = require('../config');

const mqttClient = mqtt.connect(brokerUrl, mqttOptions);
mqttClient.on('connect', () => console.log('🔗 Connected to MQTT broker'));
mqttClient.on('error', (err) => console.error('❌ MQTT Error:', err));

module.exports = { mqttClient };
