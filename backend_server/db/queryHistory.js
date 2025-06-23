const dbClient = require('./client');

function insertEntryData(sensorName, sensorValue) {
  return dbClient.query('INSERT INTO entry_history(sensorname, sensorvalue) VALUES ($1, $2)', [sensorName, sensorValue]);
}

function getEntryData() {
  return dbClient.query('SELECT * FROM entry_history ORDER BY timestamp DESC LIMIT 10');
}

module.exports = { insertEntryData, getEntryData };
