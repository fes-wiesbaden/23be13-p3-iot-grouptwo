const dbClient = require('./client');

function insertEntryData(sensorName, sensorValue) {
  return dbClient.query('INSERT INTO entry_history(sensorname, sensorvalue) VALUES ($1, $2)', [sensorName, sensorValue]);
}

function insertBuzzerData(value){
  return dbClient.query('INSERT INTO buzzer_events(value) VALUES ($1)', [value]);
}
function insertAccessAttemptsData(sensor_name, value){
  return dbClient.query('INSERT INTO access_attempts(sensor_name, value) VALUES ($1,$2)', [sensor_name, value]);
}

function insertDoorEventsData(value){
  return dbClient.query('INSERT INTO door_events(value) VALUES ($1)', [value]);
}

function insertTemperatureData(room,value){
  return dbClient.query('INSERT INTO temperature_data(room, value) VALUES ($1,$2)', [room, value]);
}

function getEntryData() {
  return dbClient.query('SELECT * FROM entry_history ORDER BY timestamp DESC LIMIT 10');
}

module.exports = { insertEntryData, insertBuzzerData,insertAccessAttemptsData, insertDoorEventsData, insertTemperatureData, getEntryData };
