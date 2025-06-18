module.exports = {
  brokerUrl: 'mqtt://smart-home.local',
  mqttOptions: {
    username: 'admin',
    password: 'admin',
    clientId: 'nodejs-controller-' + Math.random().toString(16).substr(2, 8),
  },
  dbConfig: {
    user: 'admin',
    password: 'admin',
    host: '10.93.131.8',
    port: 5432,
    database: 'smarthomedb',
  },
  DEVICE_TYPES: ['led', 'lcd', 'servo', 'fan', 'buzzer', 'lamp', 'flameSensore'],
};
