const { insertSensorData } = require('../db/queries');
const { insertEntryData, insertBuzzerData, insertAccessAttemptsData, insertDoorEventsData, insertTemperatureData } = require('../db/queryHistory');  
const { broadcastToClients } = require('../utils/broadcast');

module.exports = function setupMQTTHandlers(mqttClient, wss) {
  const deviceTopics = [
    'device/led/#', 'device/lcd/#', 'device/servo/#', 'device/fan/#', 
    'device/buzzer/#', 'device/lamp/#', 'SF/FlameSensore'
  ];

  deviceTopics.forEach(topic => {
    mqttClient.subscribe(topic, () => console.log(`📡 Subscribed to ${topic}`));
  });

  mqttClient.subscribe('SF/TEMP', () => console.log('📡 Subscribed to SF/TEMP'));
  mqttClient.subscribe('SF/HUMIDITY', () => console.log('📡 Subscribed to SF/HUMIDITY'));
  mqttClient.subscribe('device/pinpad/pinpad', () => console.log('📡 Subscribed to device/pinpad/pinpad'));
  mqttClient.subscribe('device/buzzer/buzzer', () => console.log('📡 Subscribed to device/buzzer/buzzer'));
  mqttClient.subscribe('device/bell/bell', () => console.log('📡 Subscribed to device/bell/bell'));
  mqttClient.subscribe('SF/FlameSensore', () => console.log('📡 Subscribed to SF/FlameSensore'));
  mqttClient.subscribe('smartlock/status', () => console.log('📡 Subscribed to smartlock/status'));
  mqttClient.subscribe('device/fingerprint/fingerprint', () => console.log('📡 Subscribed to device/fingerprint/fingerprint'));
  mqttClient.subscribe('device/servo/servo', () => console.log('📡 Subscribed to device/servo/servo'));
  mqttClient.subscribe('Grage/Tor', () => console.log('📡 Subscribed to Grage/Tor'));



  mqttClient.on('message', async (topic, message) => {
    const value = message.toString();
    if (value != "ONLINE") {
      switch (topic) {
      case 'SF/TEMP':
        await insertTemperatureData('living_room', value);
        broadcastToClients(wss, { type: 'status', device: 'temp', value });
        break;

      case 'device/pinpad/pinpad':
        await insertAccessAttemptsData('pinpad', value);
        broadcastToClients(wss, { type: 'status', device: 'pinpad', value });
        break;

      case 'device/buzzer/buzzer':
        if(value == 'BuzzerON'){
          await insertAccessAttemptsData('buzzer', 'accessAllowed');
          broadcastToClients(wss, { type: 'status', device: 'buzzer', value });
        }
        else if(value == 'BuzzerOFF'){
          await insertAccessAttemptsData('buzzer','accessDenied');
          broadcastToClients(wss, { type: 'status', device: 'buzzer', value });
        }
        break;

      case 'device/bell/bell':
        await insertBuzzerData('ring');
        broadcastToClients(wss, { type: 'status', device: 'bell', value });
        break;

      case 'smartlock/status':
        await insertEntryData('smartlock', value);
        broadcastToClients(wss, { type: 'status', device: 'smartlock', value });
        break;

      case 'device/fingerprint/fingerprint':
        await insertAccessAttemptsData('fingerprint', 'fingerprint_used');
        broadcastToClients(wss, { type: 'status', device: 'fingerprint', value });
        break;

      case 'device/servo/servo':
        if(value == 'ON'){
          await insertDoorEventsData('door_open');
          broadcastToClients(wss, { type: 'status', device: 'servo', value });
        }
        else if(value == 'OFF'){
          await insertDoorEventsData('door_closed');
          broadcastToClients(wss, { type: 'status', device: 'servo', value });
        }
        break;

      case 'SF/HUMIDITY':
        broadcastToClients(wss, { type: 'status', device: 'humidity', value });
        break;
      
      case 'SF/FlameSensore':

        broadcastToClients(wss, { type: 'status', device: 'FlameSensore', value });
        console.log("***Flame Detected!!!***")
        break;

      case 'smartlock/status':
      broadcastToClients(wss, {type: 'status', device:'NumPadLock', value}); 
      console.log(value);
      break;

      case 'Grage/Tor':
        if(value== "ON"){
          await insertDoorEventsData('garage_open');
        broadcastToClients(wss, {type: 'status', device:'Grage/Tor', value});
        }else{
          await insertDoorEventsData('garade_closed');
          broadcastToClients(wss, {type: 'status', device:'Grage/Tor', value}); 
        }
        break;

      default:
        const parts = topic.split('/');
        if (parts.length === 3 && parts[0] === 'device') {
          const [, deviceType, deviceId] = parts;
          broadcastToClients(wss, { type: 'status', deviceType, deviceId, value });
        }
    }
    }
  });
};
